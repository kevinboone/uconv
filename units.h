/*============================================================================
  units.h

  (c)2013 Kevin Boone
  Distributed under the terms of the GNU Public Licence, version 2
============================================================================*/

#pragma once

// Maximum number of individual units in a compound unit
#define MAX_UNIT_ELEMENTS 10

// Maximum length of a unit token. In practice, > 4 is rare
#define MAX_UNIT_STRING 64

#ifndef BOOL
typedef int BOOL;
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef enum { meter = 1, second, hectare, foot, acre, usacre, hour, minute, day,
   inch, yard, mile, litre, gallon, usgallon, pint, uspint, gramme, pound, stone,
   tonne, ton, uston, nauticalmile, poundforce, newton, bar, pascal, psi, atmosphere, 
   mmHg, fathom, fahrenheit, celsius, kelvin, grammeforce, dyne, cmh20, joule, 
   calorie, btu, watt, horsepower, angstrom, au, knot,
   light_year, point, board, cord, wood_cord, mph, kmh, kilo, carat, grain,
   ounce, electron_volt, erg, ton_tnt, torr, quart, usquart, fluid_ounce, degree,
   radian, gradian, revolution, hundredweight, ushundredweight, troy_pound,
   troy_ounce, hand, load, therm, ampere, coulomb, faraday, becquerel, curie,
   rutherford, roentgen, gray, rad, sievert, REM, steradian, candela, candlepower,
   lambert, footlambert, lumen, lux, footcandle, phot, mpg, litreper100km, byte,
   kilobyte, megabyte, gigabyte, terabyte, petabyte, exabyte, kibibyte, mebibyte,
   gibibyte, tebibyte, pebibyte, exbibyte, bit, kilobit, megabit, gigabit, terabit,
   petabit, exabit, kibibit, mebibit, gibibit, tebibit, pebibit, exbibit}
  Unit;

typedef struct _UnitAndPower
  {
  Unit unit;
  int power;
  int prefix_power;
  } UnitAndPower;

typedef struct _Units
  {
  int n_elements;  
  UnitAndPower units[MAX_UNIT_ELEMENTS];
  } Units;


Units *units_parse (const char *text, char **error);
void units_free (Units *self);
void units_dump (const Units *self);
char *units_formt_string (const Units *self);
double units_convert (double n, const Units *from_units, 
const Units *to_units, char **error);
char *units_format_string_and_value (const Units *self, double n, 
  BOOL force_decimal);
char *units_format_string (const Units *self, BOOL plural);
void units_dump_tables (FILE *f); 


