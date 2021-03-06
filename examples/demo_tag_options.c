/*
  File autogenerated by gengetopt version 2.1  
  generated with the following command:
  gengetopt --file-name=demo_tag_options --unamed-opts --input=demo_tag_options.ggo 

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Check for configure's getopt check result.  */
#ifndef HAVE_GETOPT_LONG
#include "getopt.h"
#else
#include <getopt.h>
#endif

#include "demo_tag_options.h"

/* Don't define PACKAGE and VERSION if we use automake.  */
#define GGO_PACKAGE "id3tag"
#ifndef VERSION
#  define GGO_VERSION VERSION
#else
/* ******* WRITE THE VERSION OF YOUR PROGRAM HERE ******* */
#  define GGO_VERSION ""
#endif


void
print_version (void)
{
  printf ("%s %s\n", GGO_PACKAGE, GGO_VERSION);
}

void
print_help (void)
{
  print_version ();
  printf ("Usage: %s [OPTIONS]... [FILES]...\n\
   -h         --help            Print help and exit\n\
   -V         --version         Print version and exit\n\
   -1         --v1tag           Render only the id3v1 tag (default=off)\n\
   -2         --v2tag           Render only the id3v2 tag (default=off)\n\
   -aSTRING   --artist=STRING   Set the artist information\n\
   -ASTRING   --album=STRING    Set the album title information\n\
   -sSTRING   --song=STRING     Set the title information\n\
   -cSTRING   --comment=STRING  Set the comment information\n\
   -CSTRING   --desc=STRING     Set the comment description\n\
   -ySTRING   --year=STRING     Set the year\n\
   -tSTRING   --track=STRING    Set the track number\n\
   -TSTRING   --total=STRING    Set the total number of tracks\n\
   -gSHORT    --genre=SHORT     Set the genre\n\
   -w         --warning         Turn on warnings (for debugging) (default=off)\n\
   -n         --notice          Turn on notices (for debugging) (default=off)\n\
", GGO_PACKAGE);
}


char *
gengetopt_strdup (char * s)
{
  char * n, * pn, * ps = s;
  while (*ps) ps++;
  n = (char *) malloc (1 + ps - s);
  if (n != NULL)
    {
      for (ps=s,pn=n; *ps; ps++,pn++)
        *pn = *ps;
      *pn = 0;
    }
  return n;
}


int
cmdline_parser (int argc, char * const *argv, struct gengetopt_args_info *args_info)
{
  int c;	/* Character of the parsed option.  */
  int missing_required_options = 0;	

  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->v1tag_given = 0 ;
  args_info->v2tag_given = 0 ;
  args_info->artist_given = 0 ;
  args_info->album_given = 0 ;
  args_info->song_given = 0 ;
  args_info->comment_given = 0 ;
  args_info->desc_given = 0 ;
  args_info->year_given = 0 ;
  args_info->track_given = 0 ;
  args_info->total_given = 0 ;
  args_info->genre_given = 0 ;
  args_info->warning_given = 0 ;
  args_info->notice_given = 0 ;

#define clear_args()
{ \
  args_info->v1tag_flag = 0;\
  args_info->v2tag_flag = 0;\
  args_info->artist_arg = NULL; \
  args_info->album_arg = NULL; \
  args_info->song_arg = NULL; \
  args_info->comment_arg = NULL; \
  args_info->desc_arg = NULL; \
  args_info->year_arg = NULL; \
  args_info->track_arg = NULL; \
  args_info->total_arg = NULL; \
  args_info->warning_flag = 0;\
  args_info->notice_flag = 0;\
}

  clear_args();

  args_info->inputs = NULL;
  args_info->inputs_num = 0;

  while (1)
    {
      int option_index = 0;
      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 'V' },
        { "v1tag",	0, NULL, '1' },
        { "v2tag",	0, NULL, '2' },
        { "artist",	1, NULL, 'a' },
        { "album",	1, NULL, 'A' },
        { "song",	1, NULL, 's' },
        { "comment",	1, NULL, 'c' },
        { "desc",	1, NULL, 'C' },
        { "year",	1, NULL, 'y' },
        { "track",	1, NULL, 't' },
        { "total",	1, NULL, 'T' },
        { "genre",	1, NULL, 'g' },
        { "warning",	0, NULL, 'w' },
        { "notice",	0, NULL, 'n' },
        { NULL,	0, NULL, 0 }
      };

      c = getopt_long (argc, argv, "hV12a:A:s:c:C:y:t:T:g:wn", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          clear_args ();
          print_help ();
          exit (0);

        case 'V':	/* Print version and exit.  */
          clear_args ();
          print_version ();
          exit (0);

        case '1':	/* Render only the id3v1 tag.  */
          args_info->v1tag_flag = !(args_info->v1tag_flag);
          break;

        case '2':	/* Render only the id3v2 tag.  */
          args_info->v2tag_flag = !(args_info->v2tag_flag);
          break;

        case 'a':	/* Set the artist information.  */
          if (args_info->artist_given)
            {
              fprintf (stderr, "%s: `--artist' (`-a') option given more than once\n", GGO_PACKAGE);
              clear_args ();
              print_help ();
              exit (1);
            }
          args_info->artist_given = 1;
          args_info->artist_arg = gengetopt_strdup (optarg);
          break;

        case 'A':	/* Set the album title information.  */
          if (args_info->album_given)
            {
              fprintf (stderr, "%s: `--album' (`-A') option given more than once\n", GGO_PACKAGE);
              clear_args ();
              print_help ();
              exit (1);
            }
          args_info->album_given = 1;
          args_info->album_arg = gengetopt_strdup (optarg);
          break;

        case 's':	/* Set the title information.  */
          if (args_info->song_given)
            {
              fprintf (stderr, "%s: `--song' (`-s') option given more than once\n", GGO_PACKAGE);
              clear_args ();
              print_help ();
              exit (1);
            }
          args_info->song_given = 1;
          args_info->song_arg = gengetopt_strdup (optarg);
          break;

        case 'c':	/* Set the comment information.  */
          if (args_info->comment_given)
            {
              fprintf (stderr, "%s: `--comment' (`-c') option given more than once\n", GGO_PACKAGE);
              clear_args ();
              print_help ();
              exit (1);
            }
          args_info->comment_given = 1;
          args_info->comment_arg = gengetopt_strdup (optarg);
          break;

        case 'C':	/* Set the comment description.  */
          if (args_info->desc_given)
            {
              fprintf (stderr, "%s: `--desc' (`-C') option given more than once\n", GGO_PACKAGE);
              clear_args ();
              print_help ();
              exit (1);
            }
          args_info->desc_given = 1;
          args_info->desc_arg = gengetopt_strdup (optarg);
          break;

        case 'y':	/* Set the year.  */
          if (args_info->year_given)
            {
              fprintf (stderr, "%s: `--year' (`-y') option given more than once\n", GGO_PACKAGE);
              clear_args ();
              print_help ();
              exit (1);
            }
          args_info->year_given = 1;
          args_info->year_arg = gengetopt_strdup (optarg);
          break;

        case 't':	/* Set the track number.  */
          if (args_info->track_given)
            {
              fprintf (stderr, "%s: `--track' (`-t') option given more than once\n", GGO_PACKAGE);
              clear_args ();
              print_help ();
              exit (1);
            }
          args_info->track_given = 1;
          args_info->track_arg = gengetopt_strdup (optarg);
          break;

        case 'T':	/* Set the total number of tracks.  */
          if (args_info->total_given)
            {
              fprintf (stderr, "%s: `--total' (`-T') option given more than once\n", GGO_PACKAGE);
              clear_args ();
              print_help ();
              exit (1);
            }
          args_info->total_given = 1;
          args_info->total_arg = gengetopt_strdup (optarg);
          break;

        case 'g':	/* Set the genre.  */
          if (args_info->genre_given)
            {
              fprintf (stderr, "%s: `--genre' (`-g') option given more than once\n", GGO_PACKAGE);
              clear_args ();
              print_help ();
              exit (1);
            }
          args_info->genre_given = 1;
          args_info->genre_arg = (short)atoi (optarg);
          break;

        case 'w':	/* Turn on warnings (for debugging).  */
          args_info->warning_flag = !(args_info->warning_flag);
          break;

        case 'n':	/* Turn on notices (for debugging).  */
          args_info->notice_flag = !(args_info->notice_flag);
          break;

        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          exit (1);

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c\n", GGO_PACKAGE, c);
          abort ();
        } /* switch */
    } /* while */

  if ( missing_required_options )
    exit (1);

  if (optind < argc)
    {
      int i = 0 ;

      args_info->inputs_num = argc - optind ;
      args_info->inputs = 
        (char **)(malloc ((args_info->inputs_num)*sizeof(char *))) ;
      while (optind < argc)
        args_info->inputs[ i++ ] = strdup (argv[optind++]) ; 
    }

  return 0;
}
