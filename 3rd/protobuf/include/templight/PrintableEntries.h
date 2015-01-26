/**
 * \file PrintableEntries.h
 *
 * This library provides the basic classes for creating writers that can print 
 * templight traces into some specific format.
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

#ifndef TEMPLIGHT_PRINTABLE_ENTRIES_H
#define TEMPLIGHT_PRINTABLE_ENTRIES_H

#include <ostream>
#include <string>
#include <cstdint>

namespace templight {

/** \brief Represents the beginning of a templight trace entry.
 * 
 * This struct represents the beginning of a templight trace entry, 
 * holding the kind of instantiation, the template's name (full), 
 * the location of the instantiation, the location of origin of the 
 * template (template's definition), and the absolute compilation costs 
 * at the point of instantiation (difference with the ending part gives 
 * the actual cost of the instantiation).
 */
struct PrintableEntryBegin {
  int InstantiationKind; ///< The kind of instantiation that this entry represents.
  std::string Name;      ///< The name of the template instantiation.
  std::string FileName;  ///< The filename where the instantiation occurred.
  int Line;              ///< The line where the instantiation occurred.
  int Column;            ///< The column where the instantiation occurred.
  double TimeStamp;          ///< The time-stamp at the beginning of the instantiation.
  std::uint64_t MemoryUsage; ///< The memory usage at the beginning of the instantiation.
  std::string TempOri_FileName; ///< The filename where the template is defined.
  int TempOri_Line;             ///< The line where the template is defined.
  int TempOri_Column;           ///< The column where the template is defined.
};

/** \brief Represents the end of a templight trace entry.
 * 
 * This struct represents the end of a templight trace entry, 
 * holding the absolute compilation costs at the point of 
 * instantiation (difference with the beginning part gives 
 * the actual cost of the instantiation).
 */
struct PrintableEntryEnd {
  double TimeStamp;          ///< The time-stamp at the end of the instantiation.
  std::uint64_t MemoryUsage; ///< The memory usage at the end of the instantiation.
};


/** \brief Base class for entry writers.
 * 
 * This base-class handles the actual writing of the templight trace 
 * entries to an output stream. Derived classes do the actual writing 
 * and formating of the output.
 */
class EntryWriter {
public:
  
  /** \brief Creates a writer for the given output stream.
   * 
   * Creates an entry-writer for the given output stream.
   */
  EntryWriter(std::ostream& aOS) : OutputOS(aOS) { };
  virtual ~EntryWriter() { };
  
  /** \brief Initializes the writer with a given source filename.
   * 
   * Initializes the writer with a given source filename. This often 
   * does the writing of the header information for the format implemented 
   * by the derived class.
   * \param aSourceName The filename of the source file that generated the trace being printed.
   */
  virtual void initialize(const std::string& aSourceName = "") = 0;
  
  /** \brief Finalizes the writer.
   * 
   * Finalizes the writer. This often 
   * does the writing of the closing tags or information for the format implemented 
   * by the derived class.
   */
  virtual void finalize() = 0;
  
  /** \brief Prints a beginning entry.
   * 
   * This function prints the beginning part of a templight trace entry.
   * \param aEntry The beginning part of a templight trace entry.
   */
  virtual void printEntry(const PrintableEntryBegin& aEntry) = 0;
  
  /** \brief Prints an end entry.
   * 
   * This function prints the end part of a templight trace entry.
   * \param aEntry The end part of a templight trace entry.
   */
  virtual void printEntry(const PrintableEntryEnd& aEntry) = 0;
  
protected:
  std::ostream& OutputOS;
};


}

#endif



