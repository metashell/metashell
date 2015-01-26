/*
 *    Copyright 2015 Sven Mikael Persson
 *
 *    THIS SOFTWARE IS DISTRIBUTED UNDER THE TERMS OF THE GNU GENERAL PUBLIC LICENSE v3 (GPLv3).
 *
 *    This file is part of templight-tools.
 *
 *    Templight-tools is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Templight-tools is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with templight-tools (as LICENSE in the root folder).
 *    If not, see <http://www.gnu.org/licenses/>.
 */

#include <templight/ProtobufReader.h>
#include <templight/ThinProtobuf.h>

#include <vector>
#include <algorithm>
#include <string>
#include <cstdint>

namespace templight {


ProtobufReader::ProtobufReader() { }

void ProtobufReader::loadHeader(std::streampos buf_limit) {
  // Set default values:
  Version = 0;
  SourceName = "";
  
  while ( buffer->tellg() < buf_limit ) {
    unsigned int cur_wire = thin_protobuf::loadVarInt(*buffer);
    switch( cur_wire ) {
      case thin_protobuf::getVarIntWire<1>::value:
        Version = thin_protobuf::loadVarInt(*buffer);
        break;
      case thin_protobuf::getStringWire<2>::value:
        SourceName = thin_protobuf::loadString(*buffer);
        break;
      default:
        thin_protobuf::skipData(*buffer, cur_wire);
        break;
    }
  }
  
  LastChunk = ProtobufReader::Header;
}

void ProtobufReader::loadDictionaryEntry(std::streampos buf_limit) {
  // Set default values:
  std::string name = "";
  std::vector<std::size_t> markers;
  
  while ( buffer->tellg() < buf_limit ) {
    unsigned int cur_wire = thin_protobuf::loadVarInt(*buffer);
    switch( cur_wire ) {
      case thin_protobuf::getStringWire<1>::value:
        name = thin_protobuf::loadString(*buffer);
        break;
      case thin_protobuf::getVarIntWire<2>::value:
        markers.push_back(thin_protobuf::loadVarInt(*buffer));
        break;
      default:
        thin_protobuf::skipData(*buffer, cur_wire);
        break;
    }
  }
  
  std::string::iterator it_name = std::find(name.begin(), name.end(), '\0');
  std::vector<std::size_t>::iterator it_mark = markers.begin();
  while ( ( it_name != name.end() ) && ( it_mark != markers.end() ) ) {
    std::size_t offset = it_name - name.begin();
    name.replace(it_name, it_name + 1, templateNameMap[*it_mark]);
    it_name = std::find(name.begin() + offset, name.end(), '\0');
    ++it_mark;
  }
  
  templateNameMap.push_back(name);
  
}

static void loadLocation(std::istream* buffer, 
                         std::streampos buf_limit, 
                         std::vector<std::string>& fileNameMap,
                         std::string& FileName, int& Line, int& Column) {
  // Set default values:
  FileName = "";
  std::size_t FileID = std::numeric_limits<std::size_t>::max();
  Line = 0;
  Column = 0;
  
  while ( buffer->tellg() < buf_limit ) {
    unsigned int cur_wire = thin_protobuf::loadVarInt(*buffer);
    switch( cur_wire ) {
      case thin_protobuf::getStringWire<1>::value:
        FileName = thin_protobuf::loadString(*buffer);
        break;
      case thin_protobuf::getVarIntWire<2>::value:
        FileID = thin_protobuf::loadVarInt(*buffer);
        break;
      case thin_protobuf::getVarIntWire<3>::value:
        Line = thin_protobuf::loadVarInt(*buffer);
        break;
      case thin_protobuf::getVarIntWire<4>::value:
        Column = thin_protobuf::loadVarInt(*buffer);
        break;
      default:
        thin_protobuf::skipData(*buffer, cur_wire);
        break;
    }
  }
  
  if ( FileID != std::numeric_limits<std::size_t>::max() ) {
    if ( fileNameMap.size() <= FileID )
      fileNameMap.resize(FileID + 1);
    if ( !FileName.empty() ) {
      fileNameMap[FileID] = FileName;  // overwrite existing names, if any, but there shouldn't be.
    } else {
      FileName = fileNameMap[FileID];
    }
  } // else we don't care?
  
}

void ProtobufReader::loadTemplateName(std::streampos buf_limit) {
  // Set default values:
  LastBeginEntry.Name = "";
  
  while ( buffer->tellg() < buf_limit ) {
    unsigned int cur_wire = thin_protobuf::loadVarInt(*buffer);
    switch( cur_wire ) {
      case thin_protobuf::getStringWire<1>::value:
        LastBeginEntry.Name = thin_protobuf::loadString(*buffer);
        break;
#if 0
      case thin_protobuf::getStringWire<2>::value: {
        LastBeginEntry.Name = thin_protobuf::loadString(*buffer);
        llvm::SmallVector<char,32> UBuf;
        if ( llvm::zlib::uncompress(LastBeginEntry.Name, UBuf, LastBeginEntry.Name.size() * 2) 
                == llvm::zlib::StatusOK )
          LastBeginEntry.Name.assign(UBuf.begin(), UBuf.end());
        else
          LastBeginEntry.Name = "";
        break;
      }
#endif
      case thin_protobuf::getVarIntWire<3>::value: {
        LastBeginEntry.Name = templateNameMap[thin_protobuf::loadVarInt(*buffer)];
        break;
      }
      default:
        thin_protobuf::skipData(*buffer, cur_wire);
        break;
    }
  }
  
}

void ProtobufReader::loadBeginEntry(std::streampos buf_limit) {
  // Set default values:
  LastBeginEntry.InstantiationKind = 0;
  LastBeginEntry.Name = "";
  LastBeginEntry.TimeStamp = 0.0;
  LastBeginEntry.MemoryUsage = 0;
  
  while ( buffer->tellg() < buf_limit ) {
    unsigned int cur_wire = thin_protobuf::loadVarInt(*buffer);
    switch( cur_wire ) {
      case thin_protobuf::getVarIntWire<1>::value:
        LastBeginEntry.InstantiationKind = thin_protobuf::loadVarInt(*buffer);
        break;
      case thin_protobuf::getStringWire<2>::value: {
        std::uint64_t cur_size = thin_protobuf::loadVarInt(*buffer);
        loadTemplateName(buffer->tellg() + std::streamoff(cur_size));
        break;
      }
      case thin_protobuf::getStringWire<3>::value: {
        std::uint64_t cur_size = thin_protobuf::loadVarInt(*buffer);
        loadLocation(buffer, buffer->tellg() + std::streamoff(cur_size), fileNameMap, 
          LastBeginEntry.FileName, LastBeginEntry.Line, LastBeginEntry.Column);
        break;
      }
      case thin_protobuf::getDoubleWire<4>::value:
        LastBeginEntry.TimeStamp = thin_protobuf::loadDouble(*buffer);
        break;
      case thin_protobuf::getVarIntWire<5>::value:
        LastBeginEntry.MemoryUsage = thin_protobuf::loadVarInt(*buffer);
        break;
      case thin_protobuf::getStringWire<6>::value: {
        std::uint64_t cur_size = thin_protobuf::loadVarInt(*buffer);
        loadLocation(buffer, buffer->tellg() + std::streamoff(cur_size), fileNameMap, 
          LastBeginEntry.TempOri_FileName, LastBeginEntry.TempOri_Line, LastBeginEntry.TempOri_Column);
        break;
      }
      default:
        thin_protobuf::skipData(*buffer, cur_wire);
        break;
    }
  }
  
  LastChunk = ProtobufReader::BeginEntry;
}

void ProtobufReader::loadEndEntry(std::streampos buf_limit) {
  // Set default values:
  LastEndEntry.TimeStamp = 0.0;
  LastEndEntry.MemoryUsage = 0;
  
  while ( buffer->tellg() < buf_limit ) {
    unsigned int cur_wire = thin_protobuf::loadVarInt(*buffer);
    switch( cur_wire ) {
      case thin_protobuf::getDoubleWire<1>::value:
        LastEndEntry.TimeStamp = thin_protobuf::loadDouble(*buffer);
        break;
      case thin_protobuf::getVarIntWire<2>::value:
        LastEndEntry.MemoryUsage = thin_protobuf::loadVarInt(*buffer);
        break;
      default:
        thin_protobuf::skipData(*buffer, cur_wire);
        break;
    }
  }
  
  LastChunk = ProtobufReader::EndEntry;
}

ProtobufReader::LastChunkType 
    ProtobufReader::startOnBuffer(std::istream& aBuffer) {
  buffer = &aBuffer;
  fileNameMap.clear();
  unsigned int cur_wire = thin_protobuf::loadVarInt(*buffer);
  if ( cur_wire != thin_protobuf::getStringWire<1>::value ) {
    buffer = nullptr;
    next_start = 0;
    LastChunk = ProtobufReader::EndOfFile;
    return LastChunk;
  }
  std::uint64_t cur_size = thin_protobuf::loadVarInt(*buffer);
  next_start = buffer->tellg() + std::streamoff(cur_size);
  return next();
}

ProtobufReader::LastChunkType ProtobufReader::next() {
  if ( !buffer || !(*buffer) || (buffer->tellg() >= next_start) ) {
    if ( !buffer || !(*buffer) ) {
      buffer = nullptr;
      next_start = 0;
      LastChunk = ProtobufReader::EndOfFile;
      return LastChunk;
    } else {
      return startOnBuffer(*buffer);
    }
  }
  unsigned int cur_wire = thin_protobuf::loadVarInt(*buffer);
  switch(cur_wire) {
    case thin_protobuf::getStringWire<1>::value: {
      std::uint64_t cur_size = thin_protobuf::loadVarInt(*buffer);
      loadHeader(buffer->tellg() + std::streamoff(cur_size));
      return LastChunk;
    };
    case thin_protobuf::getStringWire<2>::value: {
      std::uint64_t cur_size = thin_protobuf::loadVarInt(*buffer);
      std::streampos cur_limit = buffer->tellg() + std::streamoff(cur_size);
      cur_wire = thin_protobuf::loadVarInt(*buffer);
      cur_size = thin_protobuf::loadVarInt(*buffer);
      switch( cur_wire ) {
        case thin_protobuf::getStringWire<1>::value:
          loadBeginEntry(cur_limit);
          break;
        case thin_protobuf::getStringWire<2>::value:
          loadEndEntry(cur_limit);
          break;
        default: // ignore for fwd-compat.
          break;
      };
      return LastChunk;
    };
    case thin_protobuf::getStringWire<3>::value: {
      std::uint64_t cur_size = thin_protobuf::loadVarInt(*buffer);
      loadDictionaryEntry(buffer->tellg() + std::streamoff(cur_size));
      LastChunk = ProtobufReader::Other;
      return LastChunk;
    };
    default: { // ignore for fwd-compat.
      thin_protobuf::skipData(*buffer, cur_wire);
      return next(); // tail-call
    };
  }
}


} // namespace templight

