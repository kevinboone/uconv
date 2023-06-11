/*============================================================================
  uconv.c

  (c)2013-2021 Kevin Boone
  Distributed under the terms of the GNU Public Licence, version 2
============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "units.h" 

typedef enum {
  no_prefix,
  iec_prefix,
  si_prefix,
  digital_storage_prefix_enum_count,
} DigitalStoragePrefixType;


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
  fprintf (out, "  -s                Use powers of 10 instead of 2 for bytes and bits\n");
  fprintf (out, "  -v                Show version\n");
  }


/*============================================================================
  data_unit_type
============================================================================*/
int data_unit_type (Unit u)
  {
  switch (u)
    {
    case kibibyte:
    case mebibyte:
    case gibibyte:
    case tebibyte:
    case pebibyte:
    case exbibyte:
    case kibibit:
    case mebibit:
    case gibibit:
    case tebibit:
    case pebibit:
    case exbibit:
      return iec_prefix;

    case kilobyte:
    case megabyte:
    case gigabyte:
    case terabyte:
    case petabyte:
    case exabyte:
    case kilobit:
    case megabit:
    case gigabit:
    case terabit:
    case petabit:
    case exabit:
      return si_prefix;

    default:
      return no_prefix;
    }
  }


/*============================================================================
  si_to_iec
============================================================================*/
int si_to_iec (Unit u)
  {
  switch (u)
    {
    case kilobyte: return kibibyte;
    case megabyte: return mebibyte;
    case gigabyte: return gibibyte;
    case terabyte: return tebibyte;
    case petabyte: return pebibyte;
    case exabyte: return exbibyte;
    case kilobit: return kibibit;
    case megabit: return mebibit;
    case gigabit: return gibibit;
    case terabit: return tebibit;
    case petabit: return pebibit;
    case exabit: return exbibit;
    default: return u;
    }
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
  BOOL default_to_iec = TRUE;

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
              case 's':
                default_to_iec =FALSE;
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
          // When defaulting to IEC units, only convert to IEC units if all
          // inputs are SI units. This allows conversion of SI to IEC by mixing
          // unit types e.g. "10 gb gib".
          if (default_to_iec)
            {
            int counts[digital_storage_prefix_enum_count] = {0};

            for (i = 0; i < fu->n_elements; i++)
              counts[data_unit_type (fu->units[i].unit)]++;

            for (i = 0; i < tu->n_elements; i++)
              counts[data_unit_type (tu->units[i].unit)]++;

            if (counts[si_prefix] && !counts[iec_prefix])
              {
              for (i = 0; i < fu->n_elements; i++)
                fu->units[i].unit = si_to_iec (fu->units[i].unit);

              for (i = 0; i < tu->n_elements; i++)
                tu->units[i].unit = si_to_iec (tu->units[i].unit);
              }
            }

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

