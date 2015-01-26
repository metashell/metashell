/**
 * \file ThinProtobuf.h
 *
 * This library provides a number of low-level encoding functions for reading and writing 
 * data to a Google protocol buffer format. In other words, this is a kind of light-weight 
 * substitute for Google's official protobuf library (and the protoc compiler), which avoids 
 * that dependency and allows for a simpler reader / writer implementation that doesn't rely 
 * on the compilation of protobuf message definition files. This leads to cleaner, simpler 
 * and faster code than the often overkill solutions based on Google's protobuf library.
 * 
 * \note This header is generally intended to be included in a cpp file implementing the 
 * protobuf reading / writing code, as it contains static, inline and unnamed namespace 
 * elements. There is no danger in including it in a header, it's just that there is no 
 * point to that (encoding / decoding functions are an implementation detail, after all!).
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

#ifndef TEMPLIGHT_THIN_PROTOBUF_H
#define TEMPLIGHT_THIN_PROTOBUF_H

#include <ostream>
#include <istream>
#include <string>
#include <cstdint>

namespace thin_protobuf {

namespace {

#define TPROTO_ORDER_LITTLE_ENDIAN 1
#define TPROTO_ORDER_BIG_ENDIAN 2
#define TPROTO_ORDER_PDP_ENDIAN 3

#ifdef __GNUC__

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define TPROTO_BYTE_ORDER TPROTO_ORDER_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define TPROTO_BYTE_ORDER TPROTO_ORDER_BIG_ENDIAN
#else
#define TPROTO_BYTE_ORDER TPROTO_ORDER_PDP_ENDIAN
#endif

#endif // __GNUC__

#ifdef _MSC_VER
// Windows only support little-endian platforms:
#define TPROTO_BYTE_ORDER TPROTO_ORDER_LITTLE_ENDIAN
#endif // _MSC_VER


union float_to_ulong {
  float    f;
  std::uint32_t ui32;
};

union double_to_ulong {
  double   d;
  std::uint64_t ui64;
  std::uint32_t ui32[2];
};

union int64_to_uint64 {
  std::int64_t i64;
  std::uint64_t ui64;
};

union ulong_to_uword {
  std::uint32_t ui32;
  std::uint16_t ui16[2];
  std::uint8_t ui8[4];
};


void le2h_1ui32(std::uint32_t& value) {
#if TPROTO_BYTE_ORDER == TPROTO_ORDER_BIG_ENDIAN
  ulong_to_uword tmp; tmp.ui32 = value;
  std::uint8_t tmp_b = tmp.ui8[0];
  tmp.ui8[0] = tmp.ui8[3];
  tmp.ui8[3] = tmp_b;
  tmp_b = tmp.ui8[1];
  tmp.ui8[1] = tmp.ui8[2];
  tmp.ui8[2] = tmp_b;
  value = tmp.ui32;
#elif TPROTO_BYTE_ORDER == TPROTO_ORDER_PDP_ENDIAN
  ulong_to_uword tmp; tmp.ui32 = value;
  std::uint16_t tmp2 = tmp.ui16[0];
  tmp.ui16[0] = tmp.ui16[1];
  tmp.ui16[1] = tmp2;
  value = tmp.ui32;
#endif
};

void le2h_2ui32(double_to_ulong& value) {
#if TPROTO_BYTE_ORDER == TPROTO_ORDER_BIG_ENDIAN
  le2h_1ui32(value.ui32[0]);
  le2h_1ui32(value.ui32[1]);
  std::uint32_t tmp = value.ui32[0];
  value.ui32[0] = value.ui32[1];
  value.ui32[1] = tmp;
#endif
};


}

/** \brief Loads a single variable-length integer (uint32, uint64) from an input stream.
 * 
 * Loads a single variable-length integer (uint32, uint64) from an input stream.
 * \param p_buf An input stream from which to read.
 * \return The variable-length integer (uint32, uint64) that was read from the input stream.
 */
inline std::uint64_t loadVarInt(std::istream& p_buf) {
  std::uint64_t u = 0;
  if ( !p_buf )
    return u;
  std::uint8_t shifts = 0;
  char c;
  while( p_buf.get(c) ) {
    u |= (c & 0x7F) << shifts;
    if( !(c & 0x80) )
      return u;
    shifts += 7;
  };
  return u;
}

/** \brief Meta-function to get the wire value for a variable-length integer with a given tag number.
 * 
 * This meta-function gives a compile-time wire value corresponding to a variable-length 
 * integer field (uint32, uint64) with a given tag number. In other words, 
 * if the protobuf message has a field like: "optional uint32 foo = 3;", then 
 * the wire value would be "getVarIntWire<3>::value".
 */
template <unsigned int tag>
struct getVarIntWire {
  static const unsigned int value = (tag << 3);
};

/** \brief Meta-function to get the wire value for a variable-length integer with a given tag number.
 * 
 * This meta-function gives a compile-time wire value corresponding to a variable-length 
 * integer field (uint32, uint64) with a given tag number. In other words, 
 * if the protobuf message has a field like: "optional uint32 foo = 3;", then 
 * the wire value would be "getIntWire<3>::value".
 */
template <unsigned int tag>
struct getIntWire {
  static const unsigned int value = (tag << 3);
};

/** \brief Loads a single signed integer (int32, int64, sint32, sint64) from an input stream.
 * 
 * Loads a single signed integer (int32, int64, sint32, sint64) from an input stream.
 * \param p_buf An input stream from which to read.
 * \return The signed integer (int32, int64, sint32, sint64) that was read from the input stream.
 */
inline std::int64_t loadSInt(std::istream& p_buf) {
  std::uint64_t u = loadVarInt(p_buf);
  return (u >> 1) ^ (-static_cast<std::uint64_t>(u & 1));
}

/** \brief Meta-function to get the wire value for a signed integer with a given tag number.
 * 
 * This meta-function gives a compile-time wire value corresponding to a signed integer 
 * field (int32, int64, sint32, sint64) with a given tag number. In other words, 
 * if the protobuf message has a field like: "optional int64 foo = 4;", then 
 * the wire value would be "getSIntWire<4>::value".
 */
template <unsigned int tag>
struct getSIntWire {
  static const unsigned int value = (tag << 3);
};

/** \brief Loads a single double (fixed64, sfixed64, double) from an input stream.
 * 
 * Loads a single double (fixed64, sfixed64, double) from an input stream.
 * \param p_buf An input stream from which to read.
 * \return The double (fixed64, sfixed64, double) that was read from the input stream.
 */
inline double loadDouble(std::istream& p_buf) {
  double_to_ulong tmp;
  if( ! p_buf.read(reinterpret_cast<char*>(&tmp), sizeof(double)) )
    return 0.0;
  le2h_2ui32(tmp);
  return tmp.d;
}

/** \brief Meta-function to get the wire value for a double with a given tag number.
 * 
 * This meta-function gives a compile-time wire value corresponding to a double 
 * field (fixed64, sfixed64, double) with a given tag number. In other words, 
 * if the protobuf message has a field like: "optional double foo = 3;", then 
 * the wire value would be "getFloatWire<3>::value".
 */
template <unsigned int tag>
struct getDoubleWire {
  static const unsigned int value = (tag << 3) | 1;
};

/** \brief Loads a single float (fixed32, sfixed32, float) from an input stream.
 * 
 * Loads a single float (fixed32, sfixed32, float) from an input stream.
 * \param p_buf An input stream from which to read.
 * \return The float (fixed32, sfixed32, float) that was read from the input stream.
 */
inline float loadFloat(std::istream& p_buf) {
  float_to_ulong tmp;
  if( ! p_buf.read(reinterpret_cast<char*>(&tmp), sizeof(float)) )
    return 0.0;
  le2h_1ui32(tmp.ui32);
  return tmp.f;
}

/** \brief Meta-function to get the wire value for a float with a given tag number.
 * 
 * This meta-function gives a compile-time wire value corresponding to a float 
 * field (fixed32, sfixed32, float) with a given tag number. In other words, 
 * if the protobuf message has a field like: "optional float foo = 4;", then 
 * the wire value would be "getFloatWire<4>::value".
 */
template <unsigned int tag>
struct getFloatWire {
  static const unsigned int value = (tag << 3) | 5;
};

/** \brief Loads a single bool (bool) from an input stream.
 * 
 * Loads a single bool (bool) from an input stream.
 * \param p_buf An input stream from which to read.
 * \return The bool (bool) that was read from the input stream.
 */
inline bool loadBool(std::istream& p_buf) {
  if ( ! p_buf )
    return false;
  return p_buf.get();
}

/** \brief Meta-function to get the wire value for a bool with a given tag number.
 * 
 * This meta-function gives a compile-time wire value corresponding to a bool 
 * field with a given tag number. In other words, 
 * if the protobuf message has a field like: "optional bool foo = 3;", then 
 * the wire value would be "getBoolWire<3>::value".
 */
template <unsigned int tag>
struct getBoolWire {
  static const unsigned int value = (tag << 3);
};

/** \brief Loads a string (string, bytes, message, ..) from an input stream.
 * 
 * Loads a string (string, bytes, message, ..) from an input stream. This is a 
 * length-delimited field composed of the length (as a varint) and 
 * a corresponding number of bytes following it. All those bytes are 
 * read into a string that is returned by this function.
 * \note The strings are length-delimited, meaning that they can 
 *       contain zero characters in the middle of them, i.e., they 
 *       are not "C-style" null-terminated strings.
 * \param p_buf An input stream from which to read.
 * \return The string that was read from the input stream.
 */
inline std::string loadString(std::istream& p_buf) {
  unsigned int u = loadVarInt(p_buf);
  std::string s(u,'\0');
  p_buf.read(&s[0], u);
  return s;  //NRVO
}

/** \brief Meta-function to get the wire value for a string with a given tag number.
 * 
 * This meta-function gives a compile-time wire value corresponding to a string 
 * field (string, bytes, message, ..) with a given tag number. In other words, 
 * if the protobuf message has a field like: "optional string name = 1;", then 
 * the wire value would be "getStringWire<1>::value".
 */
template <unsigned int tag>
struct getStringWire {
  static const unsigned int value = (tag << 3) | 2;
};

/** \brief Skips the next chunk of data, identified by a given wire type.
 * 
 * This function skips the next chunk of data identified by a given wire value.
 * This is because the typical work-flow of reading protobuf data is to first 
 * read the wire value, match it against a number of expected wire values, and 
 * if not found, either report it as an error (unknown chunk) or simply skip 
 * the chunk (basic forward compatibility). This function is used in the latter 
 * case, when skipping a chunk.
 * \param p_buf An input stream from which to read.
 * \param wire The wire value seen, which describes the chunk of data ahead 
 *             on the input stream.
 */
inline void skipData(std::istream& p_buf, unsigned int wire) {
  switch(wire & 0x7) {
    case 0:
      loadVarInt(p_buf);
      break;
    case 1:
      p_buf.ignore(sizeof(double));
      break;
    case 2: {
      unsigned int u = loadVarInt(p_buf);
      p_buf.ignore(u);
      break;
    };
    case 5:
      p_buf.ignore(sizeof(float));
      break;
    default:
      break;
  }
}



/** \brief Saves a single variable-length integer (uint32, uint64) to an output stream.
 * 
 * Saves a single variable-length integer (uint32, uint64) to an output stream.
 * \param OS An output stream to write to.
 * \param u The variable-length integer (uint32, uint64) to write to the output stream.
 */
inline void saveVarInt(std::ostream& OS, std::uint64_t u) {
  std::uint8_t buf[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // 80-bits, supports at most a 64-bit varint.
  std::uint8_t* pbuf = buf;
  *pbuf = (u & 0x7F); u >>= 7;
  while(u) {
    *pbuf |= 0x80; // set first msb because there is more to come.
    pbuf++;
    *pbuf = (u & 0x7F); u >>= 7;
  };
  OS.write(reinterpret_cast<char*>(buf), pbuf - buf + 1);
}

/** \brief Saves a variable-length integer field (uint32, uint64) to an output stream.
 * 
 * Saves a variable-length integer field (uint32, uint64) to an output stream, with a 
 * given tag number. For example, if the field is "optional uint64 foo = 4;", then 
 * the field of value "foo_value" could be saved to a stream "OS" 
 * with "saveVarInt(OS, 4, foo_value);".
 * \param OS An output stream to write to.
 * \param tag The tag number of the field, to be written into the wire value.
 * \param u The variable-length integer (uint32, uint64) to write to the output stream.
 */
inline void saveVarInt(std::ostream& OS, unsigned int tag, std::uint64_t u) {
  saveVarInt(OS, (tag << 3)); // wire-type 0: Varint.
  saveVarInt(OS, u);
}

/** \brief Saves a variable-length integer field (uint32, uint64) to an output stream.
 * 
 * Saves a variable-length integer field (uint32, uint64) to an output stream, with a 
 * given tag number. For example, if the field is "optional uint64 foo = 4;", then 
 * the field of value "foo_value" could be saved to a stream "OS" 
 * with "saveInt(OS, 4, foo_value);".
 * \param OS An output stream to write to.
 * \param tag The tag number of the field, to be written into the wire value.
 * \param i The variable-length integer (uint32, uint64) to write to the output stream.
 */
inline void saveInt(std::ostream& OS, unsigned int tag, std::int64_t i) {
  saveVarInt(OS, (tag << 3)); // wire-type 0: Varint.
  int64_to_uint64 tmp; tmp.i64 = i;
  saveVarInt(OS, tmp.ui64);
}

/** \brief Saves a single signed integer (int32, int64, sint32, sint64) to an output stream.
 * 
 * Saves a single signed integer (int32, int64, sint32, sint64) to an output stream.
 * \param OS An output stream to write to.
 * \param i The signed integer (int32, int64, sint32, sint64) to write to the output stream.
 */
inline void saveSInt(std::ostream& OS, std::int64_t i) {
  // Apply the ZigZag encoding for the sign:
  saveVarInt(OS, (i << 1) ^ (i >> (sizeof(std::int64_t) * 8 - 1)));
}

/** \brief Saves a signed integer field (int32, int64, sint32, sint64) to an output stream.
 * 
 * Saves a signed integer field (int32, int64, sint32, sint64) to an output stream, with a 
 * given tag number. For example, if the field is "optional int64 foo = 4;", then 
 * the field of value "foo_value" could be saved to a stream "OS" 
 * with "saveSInt(OS, 4, foo_value);".
 * \param OS An output stream to write to.
 * \param tag The tag number of the field, to be written into the wire value.
 * \param i The signed integer (int32, int64, sint32, sint64) to write to the output stream.
 */
inline void saveSInt(std::ostream& OS, unsigned int tag, std::int64_t i) {
  saveVarInt(OS, (tag << 3)); // wire-type 0: Varint.
  saveSInt(OS, i);
}

/** \brief Saves a single double (fixed64, sfixed64, double) to an output stream.
 * 
 * Saves a single double (fixed64, sfixed64, double) to an output stream.
 * \param OS An output stream to write to.
 * \param d The double (fixed64, sfixed64, double) to write to the output stream.
 */
inline void saveDouble(std::ostream& OS, double d) {
  double_to_ulong tmp = { d };
  le2h_2ui32(tmp);
  OS.write(reinterpret_cast<char*>(&tmp), sizeof(double));
}

/** \brief Saves a double field (fixed64, sfixed64, double) to an output stream.
 * 
 * Saves a double field (fixed64, sfixed64, double) to an output stream, with a 
 * given tag number. For example, if the field is "optional double foo = 3;", then 
 * the field of value "foo_value" could be saved to a stream "OS" 
 * with "saveDouble(OS, 3, foo_value);".
 * \param OS An output stream to write to.
 * \param tag The tag number of the field, to be written into the wire value.
 * \param d The double (fixed64, sfixed64, double) to write to the output stream.
 */
inline void saveDouble(std::ostream& OS, unsigned int tag, double d) {
  saveVarInt(OS, (tag << 3) | 1); // wire-type 1: 64-bit.
  saveDouble(OS, d);
}

/** \brief Saves a single float (fixed32, sfixed32, float) to an output stream.
 * 
 * Saves a single float (fixed32, sfixed32, float) to an output stream.
 * \param OS An output stream to write to.
 * \param d The float (fixed32, sfixed32, float) to write to the output stream.
 */
inline void saveFloat(std::ostream& OS, float d) {
  float_to_ulong tmp = { d };
  le2h_1ui32(tmp.ui32);
  OS.write(reinterpret_cast<char*>(&tmp), sizeof(float));
}

/** \brief Saves a float field (fixed32, sfixed32, float) to an output stream.
 * 
 * Saves a float field (fixed32, sfixed32, float) to an output stream, with a 
 * given tag number. For example, if the field is "optional float foo = 3;", then 
 * the field of value "foo_value" could be saved to a stream "OS" 
 * with "saveFloat(OS, 3, foo_value);".
 * \param OS An output stream to write to.
 * \param tag The tag number of the field, to be written into the wire value.
 * \param d The float (fixed32, sfixed32, float) to write to the output stream.
 */
inline void saveFloat(std::ostream& OS, unsigned int tag, float d) {
  saveVarInt(OS, (tag << 3) | 5); // wire-type 5: 32-bit.
  saveFloat(OS, d);
}

/** \brief Saves a single bool (bool) to an output stream.
 * 
 * Saves a single bool (bool) to an output stream.
 * \param OS An output stream to write to.
 * \param b The bool (bool) to write to the output stream.
 */
inline void saveBool(std::ostream& OS, bool b) {
  char tmp = 0;
  if(b) tmp = 1;
  OS.write(&tmp, 1);
}

/** \brief Saves a bool field (bool) to an output stream.
 * 
 * Saves a bool field (bool) to an output stream, with a 
 * given tag number. For example, if the field is "optional bool foo = 3;", then 
 * the field of value "foo_value" could be saved to a stream "OS" 
 * with "saveBool(OS, 3, foo_value);".
 * \param OS An output stream to write to.
 * \param tag The tag number of the field, to be written into the wire value.
 * \param b The bool (bool) to write to the output stream.
 */
inline void saveBool(std::ostream& OS, unsigned int tag, bool b) {
  saveVarInt(OS, (tag << 3)); // wire-type 0: varint.
  saveBool(OS, b);
}

/** \brief Saves a single string (string, bytes, message, ..) to an output stream.
 * 
 * Saves a single string (string, bytes, message, ..) to an output stream.
 * \param OS An output stream to write to.
 * \param s The string field (string, bytes, message, ..) to write to the output stream.
 */
inline void saveString(std::ostream& OS, const std::string& s) {
  unsigned int u = s.size();
  saveVarInt(OS, u);
  OS.write(s.data(), u);
}

/** \brief Saves a string field (string, bytes, message, ..) to an output stream.
 * 
 * Saves a string field (string, bytes, message, ..) to an output stream, with a 
 * given tag number. For example, if the field is "optional string foo = 6;", then 
 * the field of value "foo_value" could be saved to a stream "OS" 
 * with "saveBool(OS, 6, foo_value);".
 * \param OS An output stream to write to.
 * \param tag The tag number of the field, to be written into the wire value.
 * \param s The string field (string, bytes, message, ..) to write to the output stream.
 */
inline void saveString(std::ostream& OS, unsigned int tag, const std::string& s) {
  saveVarInt(OS, (tag << 3) | 2); // wire-type 2: length-delimited.
  saveString(OS, s);
}


} // namespace thin_protobuf

#endif
