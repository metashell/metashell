/**
 * \file ProtobufReader.h
 *
 * This library provides a class for reading protobuf formatted templight trace files.
 *
 * \author S. Mikael Persson <mikael.s.persson@gmail.com>
 * \date January 2015
 */

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

#ifndef TEMPLIGHT_PROTOBUF_READER_H
#define TEMPLIGHT_PROTOBUF_READER_H

#include <templight/PrintableEntries.h>

#include <string>
#include <vector>
#include <istream>

namespace templight {

/** \brief A trace-reader for a Google protobuf format.
 * 
 * This class will read the traces from the given input stream in 
 * a Google protobuf format, as a flat sequence of begin and end entries.
 * 
 * The message definition for the protobuf format can be found at:
 * https://github.com/mikael-s-persson/templight/blob/master/templight_messages.proto
 * 
 * And the explanation of the protobuf format compression scheme can be found at:
 * https://github.com/mikael-s-persson/templight/wiki/Protobuf-Template-Name-Compression---Explained
 */
class ProtobufReader {
private:
  
  std::istream* buffer;
  std::streampos next_start;
  
  std::vector< std::string > fileNameMap;
  std::vector< std::string > templateNameMap;
  
  void loadHeader(std::streampos buf_limit);
  void loadDictionaryEntry(std::streampos buf_limit);
  void loadTemplateName(std::streampos buf_limit);
  void loadBeginEntry(std::streampos buf_limit);
  void loadEndEntry(std::streampos buf_limit);
  
public:
  
  /// Identifies what kind of chunk was last seen in the trace file (during last call to "next()")
  enum LastChunkType {
    EndOfFile = 0,  ///< Reached the end of the file / stream.
    Header,         ///< Found the header information chunk.
    BeginEntry,     ///< Found the beginning part of a templight entry.
    EndEntry,       ///< Found the ending part of a templight entry.
    Other           ///< Found something else (either unsupported or "private").
  } LastChunk; ///< Holds the record of the last kind of chunk encountered.
  
  unsigned int Version;   ///< Holds the version number of the protobuf stream.
  std::string SourceName; ///< Holds the source name of the translation unit that produced the trace.
  
  PrintableEntryBegin LastBeginEntry; ///< Holds the last beginning entry.
  PrintableEntryEnd   LastEndEntry;   ///< Holds the last end entry.
  
  /** \brief Creates a protobuf reader object.
   * 
   * This creates a protobuf reader object to read the traces contained 
   * in a given input stream.
   */
  ProtobufReader();
  
  /** \brief Starts to read a given input stream.
   * 
   * This function triggers the start of the reading of a trace from a given input stream.
   * \param aBuffer An input stream where there is a protobuf trace to read from.
   * \return The first kind of chunk found in the buffer (usually, should be Header).
   */
  LastChunkType startOnBuffer(std::istream& aBuffer);
  
  /** \brief Reads the next chunk in the input stream.
   * 
   * This function reads the next chunk of a trace from its input stream.
   * \return The kind of chunk found in the buffer, the information itself 
   *         can be retrieved from the appropriate data member, for example, 
   *         if BeginEntry is returned, then use LastBeginEntry to get the 
   *         entry information.
   */
  LastChunkType next();
  
};


}

#endif



