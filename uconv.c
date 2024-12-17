/*============================================================================
  uconv.c

  (c)2013-2021 Kevin Boone
  Distributed under the terms of the GNU Public Licence, version 2
============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
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
  printf ("Copyright (c)2013-2024 Kevin Boone and others\n");
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
  fractod
  Like strtod(3) but also handles fractions in the form of "a/b" and "a b/c".
============================================================================*/
double fractod (char *text, char **endptr)
  {
  double a, b, result = 0;
  int length = 0, x;

  if (sscanf(text, "%d %lf/%lf%n", &x, &a, &b, &length) != EOF && length)
    {
    if (a < 0 || b <= 0 || a > b)
      {
      length = 0;
      errno = EINVAL;
      }
    else
      {
      result = (double) x + (x < 0 ? -a / b : a / b);
      }
    }
  else if (sscanf(text, "%lf/%lf%n", &a, &b, &length) != EOF && length)
    {
    if (b <= 0)
      {
      length = 0;
      errno = EINVAL;
      }
    else
      result = a / b;
    }
  else if (sscanf(text, "%lf%n", &a, &length) != EOF && length)
    result = a;
  else if (*text == '\0')
    errno = EINVAL;

  if (endptr)
    *endptr = text + length;

  return result;
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

  char *from, *to, *invalid = NULL;
  double n;

  switch (argc - optind) {
    case 2:
      to = argv[optind + 1];
      errno = 0;
      n = fractod (argv[optind], &from);
      if (errno != 0 || from == argv[optind])
        {
        invalid = argv[optind];
        if (from != argv[optind]) *from = '\0'; // Don't include units in error.
        }
      else if (*from == '\0')
        {
        fprintf (stderr, "%s: Missing destination units\n", argv[0]);
        return 1;
        }
      break;
    case 3:
      from = argv[optind + 1];
      to = argv[optind + 2];
      errno = 0;
      n = fractod (argv[optind], &invalid);
      // If fractod parsed the entire string, ensure "invalid" is set to NULL.
      if (errno == 0 && invalid && *invalid == '\0') invalid = NULL;
      break;
    default:
      fprintf (stderr, "%s: Wrong number of arguments; expected 2 or 3\n",
        argv[0]);
      show_usage (argv[0], stderr);
      return 1;
  }

  if (invalid)
    {
    fprintf (stderr, "%s: %s\n", invalid,
      errno == 0 ? "Not a valid number" : strerror(errno));
    return 1;
    }

  char *error = NULL;
  Units *fu = units_parse (from, &error);
  if (fu)
    {
    Units *tu = units_parse (to, &error);
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
