/*============================================================================
  uconv.c

  (c)2013-2021 Kevin Boone
  Distributed under the terms of the GNU Public Licence, version 2
============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "units.h" 


/*============================================================================
  show_version 
============================================================================*/
void show_version (void)
  {
  printf ("%s version %s\n", NAME, VERSION);
  printf ("Copyright (c)2013-2022 Kevin Boone\n");
  printf ("Freely distrbutable under the terms of the GNU Public Licence, v3.0");
  printf ("\n");
  }


/*============================================================================
  show_usage
============================================================================*/
void show_usage (const char *argv0, FILE *out)
  {
  fprintf (out, "Usage: %s [options] {number} {from_units} {to_units}\n", argv0);
  fprintf (out, "Options:\n");
  fprintf (out, "  -d                Force decimal output\n");
  fprintf (out, "  -h                Show this message\n");
  fprintf (out, "  -l                List available units\n");
  fprintf (out, "  -v                Show version\n");
  }


/*============================================================================
  main
============================================================================*/
int main (int argc, char **argv)
  {
  int i, expected_arguments = 3, optind = 1;
  BOOL usage = FALSE;
  BOOL list = FALSE;
  BOOL version = FALSE;
  BOOL force_decimal = FALSE;

  // We have to parse the arguments manually, because the first argument
  //  might be a negative number
  for (i = 1; i < argc; i++)
    {
    if (argv[i][0] == 0) continue;
    if (argv[i][0] == '-')
      {
      if ((int)strlen (argv[i]) > 1)
        {
        if (!isdigit ((int)argv[i][1]))
          {
          int j, l = strlen (argv[i]);
          for (j = 1; j < l; j++)
            {
            switch (argv[i][j])
              {
              case 'c':
                expected_arguments = 2;
                break;
              case 'd':
                force_decimal =TRUE;
                break;
              case 'v':
                version =TRUE;
                break;
              case 'l':
                list =TRUE;
                break;
              case 'h':
                usage =TRUE;
                break;
              }
            }
          optind++;
          }
        }
      }
    else
      break;
    }

  if (usage)
    {
    show_usage (argv[0], stdout); 
    exit(0);
    }

  if (version)
    {
    show_version (); 
    exit(0);
    }
  
  if (list)
    {
    units_dump_tables (stdout); 
    exit(0);
    }

  if (argc  - optind != expected_arguments)
    {
    fprintf (stderr, "%s: Wrong number of arguments; expected %d\n",
      argv[0], expected_arguments);
    show_usage (argv[0], stderr);
    return 1;
    }

  char *from, *to, *invalid = NULL;
  double n;

  if (expected_arguments == 3)
    {
    from = argv[optind + 1];
    to = argv[optind + 2];
    if (sscanf (argv[optind], "%lf", &n) != 1) invalid = argv[optind];
    }
  else
    {
    to = argv[optind + 1];
    errno = 0;
    n = strtod (argv[optind], &from);
    if (errno != 0 || from == argv[optind])
      {
      invalid = argv[optind];
      if (from != argv[optind]) *from = '\0'; // Don't include units in error.
      }
    }

  if (invalid)
    {
    fprintf (stderr, "%s: %s\n", invalid,
      errno == 0 ? "cannot parse as argument as a number" : strerror(errno));
    return 1;
    }

  char *error = NULL;
  Units *fu = units_parse (from, &error);
  if (fu)
    {
    Units *tu = units_parse (to, &error);
    if (tu)
      {
      double res = units_convert (n, fu, tu, &error);
      if (!error)
        {
        char *fs = units_format_string_and_value (fu, n, force_decimal);
        char *ts = units_format_string_and_value (tu, res, force_decimal);
        printf ("%s = %s\n", fs, ts);
        free (fs);
        free (ts);
        return 0;
        }
      else
        {
        fprintf (stderr, "Error: %s\n", error);
        free (error);
        }
      units_free (tu);
      }
    else
      {
      fprintf (stderr, "Error: %s\n", error);
      free (error);
      }

    units_free (fu);
    }
  else
    {
    fprintf (stderr, "Error: %s\n", error);
    free (error);
    }

  return 1;
  }
