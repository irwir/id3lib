// $Id: utils.cpp,v 1.33 2003/05/10 19:31:49 t1mpy Exp $

// id3lib: a C++ library for creating and manipulating id3v1/v2 tags
// Copyright 1999, 2000  Scott Thomas Haug
// Copyright 2002 Thijmen Klok (thijmen@id3lib.org)

// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
// License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

// The id3lib authors encourage improvements and optimisations to be sent to
// the id3lib coordinator.  Please see the README file for details on where to
// send such submissions.  See the AUTHORS file for a list of people who have
// contributed to id3lib.  See the ChangeLog file for a list of changes to
// id3lib.  These files are distributed with id3lib at
// http://download.sourceforge.net/id3lib/

#include <ctype.h>

#if (defined(__GNUC__) && __GNUC__ == 2)
#  define NOCREATE ios::nocreate
#else
#  if defined(macintosh)  //not sure if this is still needed
#    define toascii(X) (X)  //not sure if this is still needed
#  endif                  //not sure if this is still needed
#  define NOCREATE ((std::ios_base::openmode)0)
#endif

#include "id3/utils.h" // has <config.h> "id3/id3lib_streams.h" "id3/globals.h" "id3/id3lib_strings.h"

#if defined HAVE_ICONV_H
   // check if we have all unicodes
#  if (defined(ID3_ICONV_FORMAT_UTF16BE) && defined(ID3_ICONV_FORMAT_UTF16) && defined(ID3_ICONV_FORMAT_UTF8) && defined(ID3_ICONV_FORMAT_ASCII))
#    include <iconv.h>
#    include <errno.h>
#  else
#    undef HAVE_ICONV_H
#  endif
#else
#  if (defined(_WIN32) && ((defined(_MSC_VER) && _MSC_VER > 1000) || (defined(__BORLANDC__) && __BORLANDC__  >= 0x0520)))
#    include <mlang.h>
#    define HAVE_MS_CONVERT
#    define ID3_MSCODEPAGE_UTF16BE   1201  //"Unicode (Big-Endian)", "unicodeFFFE", 1201
#    define ID3_MSCODEPAGE_UTF16     1200  //"Unicode", "unicode", 1200
#    define ID3_MSCODEPAGE_UTF8      65001 //"Unicode (UTF-8)", "utf-8", 65001
#    define ID3_MSCODEPAGE_ISO8859_1 28591 //"Western European (ISO)", "iso-8859-1", 28591
#  endif //if (defined(_WIN32) && defined (_MSC_VER) && _MSC_VER > 1000)
#endif //#if defined HAVE_ICONV_H

using namespace dami;

  // converts an ASCII string into a Unicode one
String mbstoucs(const String& data)
{
  size_t size = data.size();
  String unicode(size * 2, '\0');
  for (size_t i = 0; i < size; ++i)
  {
    unicode[i*2+1] = toascii(data[i]);
  }
  return unicode;
}

// converts a Unicode string into ASCII
String ucstombs(const String& data)
{
  size_t size = data.size() / 2;
  String ascii(size, '\0');
  for (size_t i = 0; i < size; ++i)
  {
    ascii[i] = toascii(data[i*2+1]);
  }
  return ascii;
}

String oldconvert(const String& data, ID3_TextEnc sourceEnc, ID3_TextEnc targetEnc)
{
  String target;
  if (ID3TE_IS_SINGLE_BYTE_ENC(sourceEnc) && ID3TE_IS_DOUBLE_BYTE_ENC(targetEnc))
  {
    target = mbstoucs(data);
  }
  else if (ID3TE_IS_DOUBLE_BYTE_ENC(sourceEnc) && ID3TE_IS_SINGLE_BYTE_ENC(targetEnc))
  {
    target = ucstombs(data);
  }
  return target;
}

#if defined(HAVE_MS_CONVERT)
UINT GetMSCodePage(ID3_TextEnc enc)
{
  switch(enc)
  {
    case ID3TE_ISO8859_1:
      return ID3_MSCODEPAGE_ISO8859_1;
    case ID3TE_UTF16: //id3lib strips the byte order, hence the actual string becomes ID3TE_UTF16BE
      //return ID3_MSCODEPAGE_UTF16; // this would be used if it had a Unicode two-byte byte order mark: ID3_MSCODEPAGE_UTF16;
    case ID3TE_UTF16BE:
      return ID3_MSCODEPAGE_UTF16BE;
    case ID3TE_UTF8:
      return ID3_MSCODEPAGE_UTF8;
    //default:
  }
   return 0;
}

String msconvert(String data, ID3_TextEnc sourceEnc, ID3_TextEnc targetEnc)
{
  String target;
  IMultiLanguage* mlang = NULL;
  IMLangConvertCharset* conv = NULL;
  HRESULT hResult = S_OK;
  UINT uiSrcCodePage = GetMSCodePage(sourceEnc);
  UINT uiDstCodePage = GetMSCodePage(targetEnc);

  if (uiSrcCodePage == uiDstCodePage) //this can happen when converting between ID3TE_UTF16BE and ID3TE_UTF16, since the byte order mark is stripped they are treated equally
    return data;

  if (uiSrcCodePage == 0 || uiDstCodePage == 0)
    return oldconvert(data, sourceEnc, targetEnc);

  //initialize com
  hResult = CoInitialize(NULL);
  if (hResult != S_OK)
  {
    CoUninitialize();
    return oldconvert(data, sourceEnc, targetEnc);
  }

  hResult = CoCreateInstance(CLSID_CMultiLanguage, NULL, CLSCTX_INPROC_SERVER, IID_IMultiLanguage, (void**) &mlang);
  if (hResult != S_OK || NULL == mlang)
  {
    CoUninitialize();
    return oldconvert(data, sourceEnc, targetEnc);
  }

  hResult = mlang->CreateConvertCharset(uiSrcCodePage, uiDstCodePage, 0, (struct IMLangConvertCharset **) &conv);
  if ( hResult != S_OK || NULL == conv )
  {
    mlang->Release();
    CoUninitialize();
    return oldconvert(data, sourceEnc, targetEnc);
  }

  unsigned char* src = (unsigned char*)data.data();
  UINT srcsize = static_cast<UINT>(data.size());
  unsigned char* dst = LEAKTESTNEW(unsigned char[2 * data.size()]);
  UINT dstsize = static_cast<UINT>((2u * data.size()) + 2u); //big enough for 1 byte to two byte conversion, plus two bytes for byte order mark

  hResult = conv->DoConversion(src, &srcsize, dst, &dstsize);
  conv->Release();
  mlang->Release();
  CoUninitialize();
  if ( hResult != S_OK )
  {
    delete[] dst;
    return oldconvert(data, sourceEnc, targetEnc);
  }

  target = (char*)dst;
  delete[] dst;
  return target;
}
#endif //defined(HAVE_MS_CONVERT)

size_t dami::renderNumber(uchar *buffer, size_t val, size_t size)
{
  size_t num = val;
  for (size_t i = 0; i < size; ++i)
  {
    buffer[size - i - 1] = (uchar)(num & MASK8);
    num >>= 8;
  }
  return size;
}

String dami::renderNumber(uint32 val, size_t size)
{
  String str(size, '\0');
  uint32 num = val;
  for (size_t i = 0; i < size; ++i)
  {
    str[size - i - 1] = (uchar)(num & MASK8);
    num >>= 8;
  }
  return str;
}


#if defined(HAVE_ICONV_H)

namespace
{
  String convert_i(iconv_t cd, String source)
  {
    String target;
    size_t source_size = source.size();
#if defined(ID3LIB_ICONV_OLDSTYLE)
    const char *source_str = source.data();
#else
    char *source_str = LEAKTESTNEW(char[source.size()+1]);
    source.copy(source_str, String::npos);
    source_str[source.length()] = 0;
#endif

#define ID3LIB_BUFSIZ 1024
    char buf[ID3LIB_BUFSIZ];
    char *target_str = buf;
    size_t target_size = ID3LIB_BUFSIZ;

    do
    {
      errno = 0;
      size_t nconv = iconv(cd,
                           &source_str, &source_size,
                           &target_str, &target_size);
      if (nconv == (size_t) -1 && errno != EINVAL && errno != E2BIG)
      {
// errno is probably EILSEQ here, which means either an invalid byte sequence or a valid but unconvertible byte sequence
#if !defined(ID3LIB_ICONV_OLDSTYLE)
        delete [] source_str;
#endif
        return target;
      }
      target.append(buf, ID3LIB_BUFSIZ - target_size);
      target_str = buf;
      target_size = ID3LIB_BUFSIZ;
    }
    while (source_size > 0);
#if !defined(ID3LIB_ICONV_OLDSTYLE)
    delete [] source_str;
#endif
    return target;
  }

  const char* getFormat(ID3_TextEnc enc)
  {
    const char* format = NULL;
    switch (enc)
    {
      case ID3TE_ISO8859_1:
        format = ID3_ICONV_FORMAT_ASCII;
        break;

      case ID3TE_UTF16:
        format = ID3_ICONV_FORMAT_UTF16;
        break;

      case ID3TE_UTF16BE:
        format = ID3_ICONV_FORMAT_UTF16BE;
        break;

      case ID3TE_UTF8:
        format = ID3_ICONV_FORMAT_UTF8;
        break;

      default:
        break;
    }
    return format;
  }
}
#endif

String dami::convert(String data, ID3_TextEnc sourceEnc, ID3_TextEnc targetEnc)
{
  String target;
  if ((sourceEnc != targetEnc) && !data.empty())
  {
#if !defined HAVE_ICONV_H
#  if defined(HAVE_MS_CONVERT)
    target = msconvert(data, sourceEnc, targetEnc);
#  else
    target = oldconvert(data, sourceEnc, targetEnc);
#  endif
#else
    const char* targetFormat = getFormat(targetEnc);
    const char* sourceFormat = getFormat(sourceEnc);

    iconv_t cd = iconv_open (targetFormat, sourceFormat);
    if (cd != (iconv_t) -1)
    {
      target = convert_i(cd, data);
      if (target.empty())
      {
        //try it without iconv
        target = oldconvert(data, sourceEnc, targetEnc);
      }
    }
    else
    {
      target = oldconvert(data, sourceEnc, targetEnc);
    }
    iconv_close (cd);
#endif
  }
  return target;
}

size_t dami::ucslen(const unicode_t *unicode)
{
  if (NULL != unicode)
  {
    for (size_t size = 0; true; size++)
    {
      if (NULL_UNICODE == unicode[size])
      {
        return size;
      }
    }
  }
  return 0;
}

namespace
{
  bool exists(const String& name)
  {
    ifstream file(name.c_str(), NOCREATE);
    return file.is_open() != 0;
  }
};

ID3_Err dami::createFile(const String& name, fstream& file)
{
  if (file.is_open())
  {
    file.close();
  }

  file.open(name.c_str(), ios::in | ios::out | ios::binary | ios::trunc);
  if (!file)
  {
    return ID3E_ReadOnly;
  }

  return ID3E_NoError;
}

size_t dami::getFileSize(fstream& file)
{
  size_t size = 0;
  if (file.is_open())
  {
    streamoff curpos = file.tellg();
    file.seekg(0, ios::end);
    size = static_cast<size_t>(file.tellg());
    file.seekg(curpos);
  }
  return size;
}

size_t dami::getFileSize(ifstream& file)
{
  size_t size = 0;
  if (file.is_open())
  {
    streamoff curpos = file.tellg();
    file.seekg(0, ios::end);
    size = static_cast<size_t>(file.tellg());
    file.seekg(curpos);
  }
  return size;
}

size_t dami::getFileSize(ofstream& file)
{
  size_t size = 0;
  if (file.is_open())
  {
    streamoff curpos = file.tellp();
    file.seekp(0, ios::end);
    size = static_cast<size_t>(file.tellp());
    file.seekp(curpos);
  }
  return size;
}

ID3_Err dami::openWritableFile(const String& name, fstream& file)
{
  if (!exists(name))
  {
    return ID3E_NoFile;
  }

  if (file.is_open())
  {
    file.close();
  }
  file.open(name.c_str(), ios::in | ios::out | ios::binary | NOCREATE);
  if (!file)
  {
    return ID3E_ReadOnly;
  }

  return ID3E_NoError;
}

ID3_Err dami::openWritableFile(const String& name, ofstream& file)
{
  if (!exists(name))
  {
    return ID3E_NoFile;
  }

  if (file.is_open())
  {
    file.close();
  }
  file.open(name.c_str(), ios::in | ios::out | ios::binary | NOCREATE);
  if (!file)
  {
    return ID3E_ReadOnly;
  }

  return ID3E_NoError;
}

ID3_Err dami::openReadableFile(const String& name, fstream& file)
{
  if (file.is_open())
  {
    file.close();
  }
  file.open(name.c_str(), ios::in | ios::binary | NOCREATE);
  if (!file)
  {
    return ID3E_NoFile;
  }

  return ID3E_NoError;
}

ID3_Err dami::openReadableFile(const String& name, ifstream& file)
{
  if (file.is_open())
  {
    file.close();
  }
  file.open(name.c_str(), ios::in | ios::binary | NOCREATE);
  if (!file)
  {
    return ID3E_NoFile;
  }

  return ID3E_NoError;
}

String dami::toString(size_t val)
{
  if (val == 0)
  {
    return "0";
  }
  String text;
  while (val > 0)
  {
    char ch = (val % 10) + '0';
    text = ch + text;
    val /= 10;
  }
  return text;
}

WString dami::toWString(const unicode_t buf[], size_t len)
{
  WString str;
  str.reserve(len);

  for (size_t i = 0; i < len; ++i)
  {
    str += static_cast<WString::value_type>(buf[i]);
  }
  return str;
}

