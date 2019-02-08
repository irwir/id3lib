// $Id: get_pic.cpp,v 1.9 2002/09/13 14:04:11 t1mpy Exp $

// id3lib: a C++ library for creating and manipulating id3v1/v2 tags
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "id3/id3lib_streams.h"
#include <stdlib.h>

#include <id3/tag.h>
#include <id3/misc_support.h>

using std::cout;
using std::endl;

int main( int argc, char *argv[])
{
  if (argc != 3)
  {
    cout << "Usage: get_pic <tagfile> <picfilename>" << endl;
    return 1;
  }

  ID3_Tag tag(argv[1]);
  const ID3_Frame* frame = tag.Find(ID3FID_PICTURE);
  if (frame && frame->Contains(ID3FN_DATA))
  {
    cout << "*** extracting picture to file \"" << argv[2] << "\"...";
    frame->Field(ID3FN_DATA).ToFile(argv[2]);
    cout << " done!" << endl;
  }
  else
  {
    cout << "*** no picture frame found in \"" << argv[1] << "\"" << endl;
    return 1;
  }

  return 0;
}

