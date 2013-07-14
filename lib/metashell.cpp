// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "metashell.hpp"
#include "forwarding_ast_consumer.hpp"
#include "metashell_ast_consumer.hpp"
#include "expression_evaluating_ast_consumer.hpp"

#include <llvm/Support/Host.h>

#include <clang/Basic/SourceManager.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/TextDiagnosticBuffer.h>
#include <clang/Parse/ParseAST.h>

using namespace metashell;

namespace
{
  template <class ASTConsumer>
  void set_ast_consumer(clang::CompilerInstance& ci_, ASTConsumer& cons_)
  {
    ci_.setASTConsumer(new forwarding_ast_consumer<ASTConsumer>(cons_));
  }

  template <class ErrorIt>
  parser_report make_report(
    ErrorIt begin_,
    ErrorIt end_,
    clang::SourceManager& sm_
  )
  {
    parser_report r;
    for (ErrorIt i = begin_; i != end_; ++i)
    {
      r.errors.push_back(i->first.printToString(sm_) + ": " + i->second);
    }
    return r;
  }

  void set_lang_opts(clang::LangOptions& langOpts_)
  {
    langOpts_.CPlusPlus = 1;
    langOpts_.CPlusPlus0x = 1;
    langOpts_.CPlusPlus1y = 1;
    langOpts_.Bool = 1;
  }

  template <class C>
  void add_paths(clang::HeaderSearchOptions& headerSearchOpts_, const C& cont_)
  {
    for (
      typename C::const_iterator i = cont_.begin(), e = cont_.end();
      i != e;
      ++i
    )
    {
      headerSearchOpts_.AddPath(
        *i,
        clang::frontend::Angled,
        true,
        false,
        false
      );
    }
  }

  void set_header_search_opts(
    clang::HeaderSearchOptions& headerSearchOpts_,
    const config& config_
  )
  {
    headerSearchOpts_.UseStandardCXXIncludes = 1;
    headerSearchOpts_.UseStandardSystemIncludes = 1;
    headerSearchOpts_.UseBuiltinIncludes = 1;
    add_paths(headerSearchOpts_, config_.include_path);
    add_paths(headerSearchOpts_, config_.sysinclude_path);
  }

  clang::DiagnosticsEngine create_diagnostics_engine(
    clang::DiagnosticConsumer* c_
  )
  {
    llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>
      ids(new clang::DiagnosticIDs());
    return clang::DiagnosticsEngine(ids, new clang::DiagnosticOptions(), c_);
  }

  clang::CompilerInvocation* create_compiler_invocation(
    clang::DiagnosticsEngine& dengine_,
    const char** arg_begin,
    const char** arg_end
  )
  {
    clang::CompilerInvocation* i = new clang::CompilerInvocation();
    clang::CompilerInvocation::CreateFromArgs(*i, arg_begin, arg_end, dengine_);
    return i;
  }

  clang::TargetInfo* build_target_info(clang::DiagnosticsEngine& diag_)
  {
    llvm::IntrusiveRefCntPtr<clang::TargetOptions>
      pto(new clang::TargetOptions());
    pto->Triple = llvm::sys::getDefaultTargetTriple();
    return clang::TargetInfo::CreateTargetInfo(diag_, *pto);
  }

  const clang::FileEntry* create_virtual_file(
    clang::FileManager& fm_,
    clang::SourceManager& sm_,
    const std::string& filename_,
    const std::string& content_
  )
  {
    // TODO: will this be deallocated?
    const llvm::MemoryBuffer* buff = llvm::MemoryBuffer::getMemBuffer(content_);
    const clang::FileEntry* file =
      fm_.getVirtualFile(
        filename_.c_str(),
        buff->getBufferSize(),
        0
      );
    sm_.overrideFileContents(file, buff);
    return file;
  }

  template <class ASTConsumer>
  void parse_source_code(
    clang::CompilerInstance& ci_,
    ASTConsumer& ast_consumer_
  )
  {
    ci_.createASTContext();
    ast_consumer_.set_context(ci_.getASTContext());
    set_ast_consumer(ci_, ast_consumer_);

    ci_.getDiagnosticClient().BeginSourceFile(
      ci_.getLangOpts(),
      &ci_.getPreprocessor()
    );
    clang::ParseAST(ci_.getPreprocessor(), &ast_consumer_, ci_.getASTContext());
    ci_.getDiagnosticClient().EndSourceFile();  
  }

  void set_source_code(
    clang::SourceManager& sm_,
    clang::FileManager& fm_,
    const std::string& src_
  )
  {
    sm_.createMainFileID(create_virtual_file(fm_, sm_, "<input>", src_));
  }

  template <class ASTConsumer>
  void parse_source_code(
    clang::CompilerInstance& ci_,
    const std::string& src_,
    ASTConsumer& ast_consumer_
  )
  {
    ci_.createFileManager();
    ci_.createSourceManager(ci_.getFileManager());
    ci_.createPreprocessor();

    set_source_code(ci_.getSourceManager(), ci_.getFileManager(), src_);

    parse_source_code(ci_, ast_consumer_);
  }

  void init_compiler_instance(
    clang::CompilerInstance& ci_,
    clang::DiagnosticsEngine& de_,
    int argc_,
    const char** argv_,
    const config& config_
  )
  {
    set_lang_opts(ci_.getLangOpts());
    set_header_search_opts(ci_.getHeaderSearchOpts(), config_);

    ci_.setInvocation(create_compiler_invocation(de_, argv_, argv_ + argc_));
    ci_.createDiagnostics(argc_, argv_, de_.getClient(), false, false);

    ci_.setTarget(build_target_info(de_));
  }

  template <class ASTConsumer>
  parser_report parse_code(
    const std::string& src_,
    ASTConsumer& ast_consumer_,
    const config& config_
  )
  {
    const char* argv[] = {"-x", "c++", "-std=c++0x"};
    const int argc = sizeof(argv) / sizeof(argv[0]);

    clang::TextDiagnosticBuffer diagnostic_buffer;
    clang::DiagnosticsEngine
      diagnostics_engine = create_diagnostics_engine(&diagnostic_buffer);

    clang::CompilerInstance ci;
    init_compiler_instance(ci, diagnostics_engine, argc, argv, config_);
    parse_source_code(ci, src_, ast_consumer_);
    
    // Take ownership of diagnostic_buffer
    diagnostics_engine.takeClient();

    return
      make_report(
        diagnostic_buffer.err_begin(),
        diagnostic_buffer.err_end(),
        ci.getSourceManager()
      );
  }
}

result metashell::validate_code(const std::string& src_, const config& config_)
{
  metashell_ast_consumer ast_consumer;

  result r;
  r.report = parse_code(src_ + "\n", ast_consumer, config_);

  return r;
}

result metashell::eval_tmp(
  const std::string& buffer_,
  const std::string& tmp_exp_,
  const config& config_
)
{
  expression_evaluating_ast_consumer ast_consumer;

  result r;

  r.report =
    parse_code(
      append_to_buffer(
        buffer_,
        " typedef " + tmp_exp_ + " __metashell_t; "
        "__metashell_t* __metashell_v;\n"
      ),
      ast_consumer,
      config_
    );

  r.output = ast_consumer.result();

  return r;
}

std::string metashell::append_to_buffer(
  const std::string& buffer_,
  const std::string& s_
)
{
  return buffer_.empty() ? s_ : (buffer_ + '\n' + s_);
}

