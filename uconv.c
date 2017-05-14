/*============================================================================
  uconv.c

  (c)2013-2017 Kevin Boone
  Distributed under the terms of the GNU Public Licence, version 2
============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "units.h" 


/*============================================================================
  show_version 
============================================================================*/
void show_version (void)
  {
  printf ("%s version %s\n", NAME, VERSION);
  printf ("Copyright (c)2013 Kevin Boone\n");
  printf ("Freely distrbutable under the terms of the GNU Public Licence");
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
  int i, optind = 1;
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
        //fprintf (stderr, "'%s -h' for help\n", argv[0]); 

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

  if (argc  - optind != 3)
    {
    fprintf (stderr, "%s: Wrong number of arguments\n", argv[0]);
    show_usage (argv[0], stderr);
    }
  else
    {
    double n;
    if (sscanf (argv[optind], "%lf", &n) == 1)
      {
      char *error = NULL;
      Units *fu = units_parse (argv[optind + 1], &error);
      if (fu)
        {
        Units *tu = units_parse (argv[optind + 2], &error);
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
      }
    else
      {
      fprintf (stderr, "Invalid number: %lf\n", n);
      }
    }

  return 0;
  }

