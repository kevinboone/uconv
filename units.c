/*============================================================================
  units.c

  (c)2013 Kevin Boone
  Distributed under the terms of the GNU Public Licence, version 2
============================================================================*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "units.h"

/*============================================================================
  conversion factors 
============================================================================*/
#define INCH_TO_METRE 0.0254
#define SQUARE(x) ((x)*(x))
#define CUBE(x) ((x)*(x)*(x))

/*============================================================================
  unit table
============================================================================*/

typedef struct _UnitTable
  {
  Unit unit; 
  const char *long_name;
  const char *alt_names;
  const char *desc;
  const char *plural_long_name;
  } UnitTable;


UnitTable unit_table [] = 
  { 
  { atmosphere, "atmosphere", "atm,atmospheres", "atmosphere" ,"atmospheres" }, 
  { acre, "acre", "acres", "acre (international)", "acres" }, 
  { angstrom, "angstrom", "angstroms,ang,angs", "angstrom", "angstroms" }, 
  { ampere, "ampere", "amperes,amp,amps,A", "ampere", "amperes" }, 
  { au, "AU", "au", "astronomical unit", "AUs" }, 
  { bar, "bar", "bar,bars", "bar" , "bars" }, 
  { becquerel, "becquerel", "becquerels,Bq,Bqs", "becquerel" , "becquerels" }, 
  { bit, "bit", "bits", "bits", "bits" },
  { board, "board", "boards,bd", "board (inch.foot)" , "boards" }, 
  { btu, "BTU", "btu", "British Thermal Unit" , "BTUs" }, 
  { byte, "byte", "bytes,b", "bytes", "bytes" },
  { calorie, "calorie", "calories,cal,cals", "calorie" , "calories" }, 
  { candlepower, "candlepower", "cp", "candlepower (new)" , "candlepower" }, 
  { candela, "candela", "candelas,cd,cds", "candela" , "candela" }, 
  { carat, "carat", "carats,ct,cts", "metric carat" , "carats" }, 
  { celsius, "celsius", "centigrade,C", "degrees celsius" , "celsius" }, 
  { cmh20, "cmH20", "cmwater,cmh20", "cm of water" , "cmH20" }, 
  { cord, "cord", "cords", "cord (area)" , "cords" }, 
  { coulomb, "coulomb", "coulombs,coul,couls", "coulomb", "coulombs" }, 
  { curie, "curie", "curies,ci", "curie", "curies" }, 
  { degree, "degree", "degrees,deg,degs", "angular degree" , "degrees" }, 
  { day, "day", "days", "day" , "days" }, 
  { dyne, "dyne", "dyn", "dyne" , "dynes" }, 
  { electron_volt, "electron-volt", "electron-volts,ev,evs", "electron volt"  ,
    "electron-volts" }, 
  { erg, "erg", "ergs", "erg" , "" }, 
  { exabit, "exabit", "exabits,ebit,ebits", "exabits", "exabits" },
  { exabyte, "exabyte", "exabytes,eb", "exabytes", "exabytes" },
  { exbibyte, "exbibyte", "exbibytes,eib", "exbibytes", "exbibyte" },
  { exbibit, "exbibit", "exbibits,eibit,eibits", "exbibits", "exbibit" },
  { fahrenheit, "fahrenheit", "fahr,F", "degrees fahrenheit" , "fahrenheit" }, 
  { faraday, "faraday", "faradays", "farady" , "faradays" }, 
  { fathom, "fathom", "fathoms,fm,fms", "fathom" , "fathoms" }, 
  { fluid_ounce, "fluid-ounce", "fluid-ounces,floz", "fluid ounce"  ,
     "fluid-ounces" }, 
  { foot, "foot", "feet,ft", "imperial foot" , "feet" }, 
  { footcandle, "footcandle", "footcandles,fc", "footcandle" , "footcandles" }, 
  { footlambert, "footlambert", "footlamberts,fL", "footlambert" , "footlamberts" }, 
  { gallon, "gallon", "gallons,gal", "imperial gallon" , "gallons" }, 
  { gibibit, "gibibit", "gibibits,gibit,gibits", "gibibits", "gibibits" },
  { gibibyte, "gibibyte", "gibibytes,gib", "gibibytes", "gibibytes" },
  { gigabit, "gigabit", "gigabits,gbit,gbits", "gigabits", "gigabits" },
  { gigabyte, "gigabyte", "gigabytes,gb", "gigabytes", "gigabytes" },
  { gradian, "gradian", "gradians,grad,grads", "angular gradian" , "gradians" }, 
  { grain, "grain", "grains,gr", "grain" , "grains" }, 
  { gramme, "gramme", "grammes,gram,grams, g, gms", "gramme" , "grammes" }, 
  { grammeforce, "gramme-force", "gforce,gf,gramforce", "gramme-force"  ,
    "grammes-force" }, 
  { gray, "gray", "grays,gy,gys", "gray" , "gray" }, 
  { hand, "hand", "hands", "hand (equestrian)" , "hands" }, 
  { hectare, "hectare", "ha,hectares", "hectare" , "hectares" }, 
  { hour, "hour", "hours,h,hr,hrs", "hour" , "hours" }, 
  { horsepower, "horsepower", "hp", "horsepower (metric)" , "horsepower" }, 
  { hundredweight, "hundredweight", "cwt", "impl. hundredweight" , 
     "hundredweight" }, 
  { inch, "inch", "inches,in", "imperial inch" , "inches" }, 
  { joule, "joule", "joules,j,J", "joule" , "joules" }, 
  { kelvin, "kelvin", "kelvins,k,K", "kelvin" , "kelvin" }, 
  { rankine, "rankine", "rankines,ra,Ra", "rankine" , "rankine" }, 
  { kibibit, "kibibit", "kibibits,kibit,kibits", "kibibits", "kibibits" },
  { kibibyte, "kibibyte", "kibibytes,kib", "kibibytes", "kibibytes" },
  { kilo, "kilo", "kilos", "kilogramme" , "kilos" }, 
  { kilobit, "kilobit", "kilobits,kbit,kbits", "kilobits", "kilobits" },
  { kilobyte, "kilobyte", "kilobytes,kb", "kilobytes", "kilobytes" },
  { kmh, "kmh", "kmh", "kilometres per hour" , "kmh" }, 
  { knot, "knot", "knots,kt,kts", "nautical knots" , "knots" }, 
  { lambert, "lambert", "lamberts,lamb,lambs", "lambert" , "lamberts" }, 
  { light_year, "light-year", "lightyear,lightyears,ly,lys", "light-year"  ,
     "light-years" }, 
  { litre, "litre", "liter,liters,litres,l", "liter" , "litres" }, 
  { litreper100km, "litreper100km", "l100k,lhk", "litres per 100 km" , 
     "litresper100km" }, 
  { load, "load", "loads", "load" , "loads" }, 
  { lumen, "lumen", "lumens,lm", "lumen" , "lumens" }, 
  { lux, "lux", "lx", "lux" , "lux" }, 
  { mebibit, "mebibit", "mebibits,mibit,mibits", "mebibits", "mebibits" },
  { mebibyte, "mebibyte", "mebibytes,mib", "mebibytes", "mebibytes" },
  { megabit, "megabit", "megabits,mbit,mbits", "megabits", "megabits" },
  { megabyte, "megabyte", "megabytes,mb", "megabytes", "megabytes" },
  { meter, "metre", "meter,meters,metres,m", "metre" , "metres" }, 
  { mile, "mile", "miles,mi", "statute mile" , "miles" }, 
  { minute, "minute", "minutes,min,mins", "minute" , "minutes" }, 
  { nauticalmile, "nautical-mile", "nautical mile,nmi,NM", 
    "nautical mile" , "nautical-miles" }, 
  { mmHg, "mmHg", "mmhg", "mm of mercury" , "mmHg" }, 
  { mpg, "mpg", "mpg", "miles per gallon (UK)" , "mpg" }, 
  { mph, "mph", "mph", "miles per hour" , "mph" }, 
  { newton, "newton", "newtons,N", "newton" , "newtons" }, 
  { ounce, "ounce", "ounces,oz", "avoirdupois ounce" , "ounces" }, 
  { pascal, "pascal", "pascal,pascals,Pa", "pascal (N/m2)" , "pascals" }, 
  { pebibit, "pebibit", "pebibits,pibit,pibits", "pebibits", "pebibits" },
  { pebibyte, "pebibyte", "pebibytes,pib", "pebibytes", "pebibytes" },
  { petabit, "petabit", "petabits,pbit,pbits", "petabits", "petabits" },
  { petabyte, "petabyte", "petabytes,pb", "petabytes", "petabytes" },
  { phot, "phot", "phots,ph", "phot" , "phots" }, 
  { pint, "pint", "pints,pt,pts", "imperial pint" , "pints" }, 
  { point, "point", "points", "type point (US, Eng)" , "points" }, 
  { pound, "pound", "pounds,lb,lbs", "imperial pound" , "pounds" }, 
  { poundforce, "pound-force", "poundforce,poundsforce,pounds-force,lbf", 
     "imperial pound-force", "pounds-force" }, 
  { psi, "psi", "PSI", "pounds per sq inch" , "psi" }, 
  { quart, "quart", "quarts,qt,qts", "imperial quart" , "quarts" }, 
  { rad, "rad", "rads", "rad" , "rads" }, 
  { radian, "radian", "radians", "angular radian" , "radians" }, 
  { REM, "REM", "REMS,rem,rems", "Roentgen equiv. man"  , "REM" }, 
  { revolution, "revolution", "revolutions,rev,revs", "angular revolution"  ,
     "revolutions" }, 
  { roentgen, "roentgen", "roentgens,R", "roentgen"  , "roentgens" }, 
  { rutherford, "rutherford", "rutherfords,rd,rds", "rutherford"  , "rutherfords" }, 
  { second, "second", "seconds,sec,secs,s", "second" , "seconds" }, 
  { sievert, "sievert", "sieverts,Sv,Svs", "sievert" , "sieverts" }, 
  { steradian, "steradian", "steradians,sterads", "solid angle" , "steradians" }, 
  { stone, "stone", "stones,st", "imperial stone" , "stones" }, 
  { tebibit, "tebibit", "tebibits,tibit,tibits", "tebibits", "tebibits" },
  { tebibyte, "tebibyte", "tebibytes,tib", "tebibytes", "tebibytes" },
  { terabit, "terabit", "terabits,tbit,tbits", "terabits", "terabits" },
  { terabyte, "terabyte", "terabytes,tb", "terabytes", "terabytes" },
  { therm, "therm", "therms", "therm" , "therms" }, 
  { ton, "ton", "tons", "imperial (long) ton" , "tons" }, 
  { ton_tnt, "ton-tnt", "tons-tnt,ttnt", "ton of TNT (energy)" , "tons-tnt" }, 
  { tonne, "tonne", "tonnes", "metric tonne" , "tonnes" }, 
  { torr, "torr", "torrs", "torr" , "torrs" }, 
  { troy_ounce, "troy-ounce", "troy-ounces,troy ounce,troy ounces,ozt", "troy ounce" , 
    "troy-ounces" }, 
  { troy_pound, "troy-pound", "troy-pounds,troy pound,troy pounds,lbt", "troy pound" , 
    "troy-pounds" }, 
  { usacre, "usacre", "usacres", "US acre" , "usacres" }, 
  { usfluid_ounce, "usfluid-ounce", "usfluid-ounces,usfloz", "US fluid ounce"  ,
     "US fluid-ounces" },
  { usgallon, "usgallon", "usgallons,usgal,usgals", "US gallon" , "usgallons" }, 
  { ushundredweight, "ushundredweight", "uscwt", "US hundredweight" , 
     "US hundredweight" }, 
  { usmpg, "usmpg", "usmpg", "miles per gallon (US)" , "usmpg" },
  { uspint, "uspint", "uspints,uspt,uspts", "US pint" , "uspints" }, 
  { usquart, "usquart", "usquarts,usqt,usqts", "US quart" , "usquarts" }, 
  { uston, "uston", "ustons", "US (short) ton" , "ustons" }, 
  { watt, "watt", "watts,w", "watt" , "watts" }, 
  { wood_cord, "wood-cord", "woodcord,woodcords,wood-cords", "wood cord (volume)"  ,
    "wood-cords" }, 
  { yard, "yard", "yards,yd,yds", "imperial yard", "yards" }, 
  { 0 } 
  };


/*============================================================================
  conversion table
============================================================================*/
typedef struct _ConvTable
  {
  Unit working_unit;
  int working_power;
  Units base_unit;
  double slope;
  } ConvTable;


ConvTable conv_table [] = 
  {
  // Temperature
  // These are only used for conversions involving rates.
  {  celsius, 1, {1, {{ fahrenheit, 1, 0}}}, 1.8 },
  {  fahrenheit, 1, {1, {{ fahrenheit, 1, 0}}}, 1 },
  {  kelvin, 1, {1, {{ fahrenheit, 1, 0}}}, 1.8 },
  {  rankine, 1, {1, {{ fahrenheit, 1, 0}}}, 1 },

  // Mass
  {  carat, 1, {1, {{ gramme, 1, 0}}}, 0.2 },
  {  grain, 1, {1, {{ gramme, 1, 0}}}, 64.79891 / 1000 },
  {  gramme, 1, {1, {{ gramme, 1, 0}}}, 1 },
  {  hundredweight, 1, {1, {{ gramme, 1, 0}}}, 50802.34544 },
  {  ounce, 1, {1, {{ gramme, 1, 0}}}, 28.349523125 },
  {  pound, 1, {1, {{ gramme, 1, 0}}}, 453.59237 },
  {  stone, 1, {1, {{ gramme, 1, 0}}}, 453.59237 * 14 },
  {  tonne, 1, {1, {{ gramme, 1, 0}}}, 1e6 },
  {  ton, 1, {1, {{ gramme, 1, 0}}}, 1016046.9088 },
  {  uston, 1, {1, {{ gramme, 1, 0}}}, 907184.74 },
  {  kilo, 1, {1, {{ gramme, 1, 0}}}, 1000 },
  {  troy_pound, 1, {1, {{ gramme, 1, 0}}}, 373.2417216 },
  {  troy_ounce, 1, {1, {{ gramme, 1, 0}}},  31.1034768 },
  {  ushundredweight, 1, {1, {{ gramme, 1, 0}}}, 45359.237 },

  // Length
  {  au, 1, {1, {{ meter, 1, 0}}}, 149597870700.0 },
  {  angstrom, 1, {1, {{ meter, 1, 0}}}, 1/1e10 },
  {  fathom, 1, {1, {{ meter, 1, 0}}}, 1.8288 },
  {  foot, 1, {1, {{ meter, 1, 0}}}, INCH_TO_METRE * 12 },
  {  hand, 1, {1, {{ meter, 1, 0}}}, 4*INCH_TO_METRE },
  {  inch, 1, {1, {{ meter, 1, 0}}}, INCH_TO_METRE },
  {  light_year, 1, {1, {{ meter, 1, 0}}}, 9.4607304725808e15 },
  {  meter, 1, {1, {{ meter, 1, 0}}}, 1 },
  {  nauticalmile, 1, {1, {{ meter, 1, 0}}}, 1853.184 },
  {  point, 1, {1, {{ meter, 1, 0}}}, 0.000351450 },
  {  mile, 1, {1, {{ meter, 1, 0}}}, INCH_TO_METRE * 36 * 1760 },
  {  yard, 1, {1, {{ meter, 1, 0}}}, INCH_TO_METRE * 36 },

  // Volume
  {  foot, 3, {1, {{ meter, 3, 0}}}, CUBE(INCH_TO_METRE * 12) },
  {  fluid_ounce, 1, {1, {{ meter, 3, 0}}}, 2.84131E-05 },
  {  gallon, 1, {1, {{ meter, 3, 0}}}, 4.54609 / 1000.0 },
  {  inch, 3, {1, {{ meter, 3, 0}}}, CUBE(INCH_TO_METRE) },
  {  litre, 1, {1, {{ meter, 3, 0}}}, 1.0/1000.0 },
  {  load, 1, {1, {{ meter, 3, 0}}}, 1.4158423296 },
  {  mile, 3, {1, {{ meter, 3, 0}}}, CUBE(INCH_TO_METRE * 36 * 1760) },
  {  pint, 1, {1, {{ meter, 3, 0}}}, 568.26125 / 1e6 },
  {  quart, 1, {1, {{ meter, 3, 0}}}, 2 * 568.26125 / 1e6 },
  {  usfluid_ounce, 1, {1, {{ meter, 3, 0}}}, 2.957353E-05 },
  {  usgallon, 1, {1, {{ meter, 3, 0}}}, 3.785411784 / 1000.0 },
  {  uspint, 1, {1, {{ meter, 3, 0}}}, 473.176473 / 1e6 },
  {  usquart, 1, {1, {{ meter, 3, 0}}}, 2 * 473.176473 / 1e6 },
  {  wood_cord, 1, {1, {{ meter, 3, 0}}}, 3.62456 },
  {  yard, 3, {1, {{ meter, 3, 0}}}, CUBE(INCH_TO_METRE * 36) },

  // Area
  {  acre, 1, {1, {{ meter, 2, 0}}}, 4046.8564224 },
  {  board, 1, {1, {{ meter, 2, 0}}}, 7.74192/1000 },
  {  cord, 1, {1, {{ meter, 2, 0}}}, 1.48644864 },
  {  foot, 2, {1, {{ meter, 2, 0}}}, SQUARE(INCH_TO_METRE * 12) },
  {  hectare, 1, {1, {{ meter, 2, 0}}}, 10000 },
  {  inch, 2, {1, {{ meter, 2, 0}}}, SQUARE(INCH_TO_METRE) },
  {  mile, 2, {1, {{ meter, 2, 0}}}, SQUARE (INCH_TO_METRE * 36 * 1760) },
  {  usacre, 1, {1, {{ meter, 2, 0}}}, 4046.87261 },
  {  yard, 2, {1, {{ meter, 2, 0}}}, SQUARE(INCH_TO_METRE * 36) },

  // Time
  {  second, 1, {1, {{ second, 1, 0}}}, 1 },
  {  hour, 1, {1, {{ second, 1, 0}}}, 3600 },
  {  minute, 1, {1, {{ second, 1, 0}}}, 60 },
  {  day, 1, {1, {{ second, 1, 0}}}, 24*3600 },

  // Force 
  {  newton, 1, {1, {{ newton, 1, 0}}}, 1 },
  {  poundforce, 1, {1, {{ newton, 1, 0}}}, 4.4482216152605 },
  {  grammeforce, 1, {1, {{ newton, 1, 0}}}, 9.80665 / 1000.0 },
  {  dyne, 1, {1, {{ newton, 1, 0}}}, 1/1e5 },

  // Pressure 
  {  pascal, 1, {2, {{newton, 1, 0}, {meter, -2, 0}}}, 1 },
  {  bar, 1, {2, {{newton, 1, 0}, {meter, -2, 0}}}, 100000 },
  {  cmh20, 1, {2, {{newton, 1, 0}, {meter, -2, 0}}}, 98.0638  },
  {  atmosphere, 1, {2, {{newton, 1, 0}, {meter, -2, 0}}}, 101325 },
  {  psi, 1, {2, {{newton, 1, 0}, {meter, -2, 0}}}, 6894.757 },
  {  mmHg, 1, {2, {{newton, 1, 0}, {meter, -2, 0}}}, 133.3224 },
  {  torr, 1, {2, {{newton, 1, 0}, {meter, -2, 0}}}, 1.333224 },

  // Energy
  {  btu, 1, {2, {{newton, 1, 0}, {meter, 1, 0}}}, 1.05505585262E3 },
  {  calorie, 1, {2, {{newton, 1, 0}, {meter, 1, 0}}}, 4.1819 },
  {  electron_volt, 1, {2, {{newton, 1, 0}, {meter, 1, 0}}}, 1.602177/1e19  },
  {  erg, 1, {2, {{newton, 1, 0}, {meter, 1, 0}}}, 1/1e7  },
  {  joule, 1, {2, {{newton, 1, 0}, {meter, 1, 0}}}, 1 },
  {  ton_tnt, 1, {2, {{newton, 1, 0}, {meter, 1, 0}}}, 4.184e9 },
  {  therm, 1, {2, {{newton, 1, 0}, {meter, 1, 0}}}, 105.505585262E6},

  // Power 
  {  watt, 1, {3, {{newton, 1, 0}, {meter, 1, 0}, {second, -1, 0}}}, 1 },
  {  horsepower, 1, {3, {{newton, 1, 0}, {meter, 1, 0}, {second, -1, 0}}}, 735.49875 },

  // Fuel economy 
  {  mpg, 1, {1, {{meter, -2, 0},}}, 354006.1899 },
  {  usmpg, 1, {1, {{meter, -2, 0},}}, 425143.7074302721 },
  {  litreper100km, 1, {1, {{meter, 2, 0},}}, 1e-8 },

  // Velocity
  {  kmh, 1, {2, {{meter, 1, 0}, {second, -1, 0}}}, 0.277777778},
  {  mph, 1, {2, {{meter, 1, 0}, {second, -1, 0}}}, 0.44704 },
  {  knot, 1, {2, {{meter, 1, 0}, {second, -1, 0}}},  0.514773 },

  // Solid angle
  {  steradian, 1, {1, {{steradian, 1, 0}}},  1 },

  // Angle
  {  radian, 1, {1, {{radian, 1, 0}}},  1 },
  {  degree, 1, {1, {{radian, 1, 0}}},  2 * M_PI / 360.0 },
  {  gradian, 1, {1, {{radian, 1, 0}}},  2 * M_PI / 400.0 },
  {  revolution, 1, {1, {{radian, 1, 0}}},  2 * M_PI },

  // Current
  {  ampere, 1, {1, {{ampere, 1, 0}}},  1 },

  // Charge -- base unit coulomb (A.s)
  {  coulomb, 1, {2, {{ampere, 1, 0}, {second, 1, 0}}}, 1 },
  {  faraday, 1, {2, {{ampere, 1, 0}, {second, 1, 0}}}, 9.64853399E4  },

  // Radioactive activity
  {  becquerel, 1, {1, {{second, -1, 0}}},  1 },
  {  curie, 1, {1, {{second, -1, 0}}},  3.7E10 },
  {  rutherford, 1, {1, {{second, -1, 0}}},  1E6 },

  // Radiation exposure
  {  roentgen, 1, {3, {{ampere, 1, 0},{second, 1, 0},{gramme, -1, 0}}},   
      2.58E-4 / 1000 },

  // Radiation dose 
  {  gray, 1, {3, {{newton, 1, 0},{meter, 1, 0},{gramme, -1, 0}}}, 0.001 },
  {  rad, 1, {3, {{newton, 1, 0},{meter, 1, 0},{gramme, -1, 0}}}, 0.001  / 100},
  {  sievert, 1, {3, {{newton, 1, 0},{meter, 1, 0},{gramme, -1, 0}}}, 0.001},
  {  REM, 1, {3, {{newton, 1, 0},{meter, 1, 0},{gramme, -1, 0}}}, 0.001 / 100},

  // Luminous intesity
  {  candela, 1, 
    {4, {{newton, 1, 0},{meter, 1, 0},{second, -1, 0},{steradian, -1, 0}}}, 
      1.0 / 683.0 },
  {  candlepower, 1, 
    {4, {{newton, 1, 0},{meter, 1, 0},{second, -1, 0},{steradian, -1, 0}}}, 
      1.0 / 683.0 },

  // Luminance
  {  lambert, 1, 
    {4, {{newton, 1, 0},{meter, -1, 0},{second, -1, 0},{steradian, -1, 0}}}, 
      3183.0988618 * 1.0 / 683.0 },
  {  footlambert, 1, 
    {4, {{newton, 1, 0},{meter, -1, 0},{second, -1, 0},{steradian, -1, 0}}}, 
      3.4262590996 * 1.0 / 683.0 },

  // Luminous flux
  {  lumen, 1, 
    {3, {{newton, 1, 0},{meter, 1, 0},{second, -1, 0}}}, 
      1.0 / 683.0 },

  // Illuminance
  {  footcandle, 1, 
    {3, {{newton, 1, 0},{meter, -1, 0},{second, -1, 0}}}, 
      10.763910417 * 1.0 / 683.0 },
  {  lux, 1, 
    {3, {{newton, 1, 0},{meter, -1, 0},{second, -1, 0}}}, 
      1.0 / 683.0 },
  {  phot, 1, 
    {3, {{newton, 1, 0},{meter, -1, 0},{second, -1, 0}}}, 
      1e4 * 1.0 / 683.0 },

  // Digital storage and transmission
  {  byte, 1, {1, {{ byte, 1, 0}}}, 1 },
  {  kilobyte, 1, {1, {{ byte, 1, 0}}}, 1e3 },
  {  megabyte, 1, {1, {{ byte, 1, 0}}}, 1e6 },
  {  gigabyte, 1, {1, {{ byte, 1, 0}}}, 1e9 },
  {  terabyte, 1, {1, {{ byte, 1, 0}}}, 1e12 },
  {  petabyte, 1, {1, {{ byte, 1, 0}}}, 1e15 },
  {  exabyte, 1, {1, {{ byte, 1, 0}}}, 1e18 },

  {  kibibyte, 1, {1, {{ byte, 1, 0}}}, 1024.0 },
  {  mebibyte, 1, {1, {{ byte, 1, 0}}}, 1048576.0 },
  {  gibibyte, 1, {1, {{ byte, 1, 0}}}, 1073741824.0 },
  {  tebibyte, 1, {1, {{ byte, 1, 0}}}, 1099511627776.0 },
  {  pebibyte, 1, {1, {{ byte, 1, 0}}}, 1125899906842624.0 },
  {  exbibyte, 1, {1, {{ byte, 1, 0}}}, 1152921504606846976.0L },

  {  bit, 1, {1, {{ byte, 1, 0}}}, 0.125 },
  {  megabit, 1, {1, {{ byte, 1, 0}}}, 125 },
  {  gigabit, 1, {1, {{ byte, 1, 0}}}, 125e3 },
  {  terabit, 1, {1, {{ byte, 1, 0}}}, 125e6 },
  {  petabit, 1, {1, {{ byte, 1, 0}}}, 125e9 },
  {  exabit, 1, {1, {{ byte, 1, 0}}}, 125e12 },

  {  kibibit, 1, {1, {{ byte, 1, 0}}}, 128.0 },
  {  mebibit, 1, {1, {{ byte, 1, 0}}}, 131072.0 },
  {  gibibit, 1, {1, {{ byte, 1, 0}}}, 134217728.0 },
  {  tebibit, 1, {1, {{ byte, 1, 0}}}, 137438953472.0 },
  {  pebibit, 1, {1, {{ byte, 1, 0}}}, 140737488355328.0 },
  {  exbibit, 1, {1, {{ byte, 1, 0}}}, 144115188075855872.0L },

  {  0 }
  };


/*============================================================================
  Special case units 
============================================================================*/

// Constants for specific units -- used to check whether to invoke specific
//  formatting rules

Units units_yard = {1, {{yard, 1, 0}}};
Units units_foot = {1, {{foot, 1, 0}}};
Units units_mile = {1, {{mile, 1, 0}}};
Units units_hour = {1, {{hour, 1, 0}}};
Units units_minute = {1, {{minute, 1, 0}}};
Units units_ton = {1, {{ton, 1, 0}}};
Units units_hundredweight = {1, {{hundredweight, 1, 0}}};
Units units_pound = {1, {{pound, 1, 0}}};
Units units_stone = {1, {{stone, 1, 0}}};
Units units_gallon = {1, {{gallon, 1, 0}}};
Units units_pint = {1, {{pint, 1, 0}}};

/*============================================================================
  unit_find_unit_by_name
============================================================================*/
Unit units_find_unit_by_name (const char *name)
  {
  int i = 0;
  while (unit_table[i].unit > 0)
    {
    if (strcasecmp (name, unit_table[i].long_name) == 0) return unit_table[i].unit;
    char *alt_names = strdup (unit_table[i].alt_names);
    char *tok = strtok (alt_names, ",");
    while (tok)
      {
      if (strcasecmp (name, tok) == 0)
        {
        free (alt_names);
        return unit_table[i].unit;
        }
  
      tok = strtok (NULL, ", ");
      }
    free (alt_names);
    i++;
    }
  return -1;
  }


/*============================================================================
  units_format_prefix_name
============================================================================*/
const char *units_format_prefix_name (int prefix_power)
  {
  switch (prefix_power)
    {
    case 0: return "";
    case -12: return "pico";
    case -9: return "nano";
    case -6: return "micro";
    case -3: return "milli";
    case -2: return "centi";
    case -1: return "deci";
    case 3: return "kilo";
    case 6: return "mega";
    case 9: return "giga";
    case 12: return "tera";
    }
  return "?";
  }


/*============================================================================
  unit_find_prefix
  Returns 0 if there is no prefix. Happily, there is no prefix for 10^0
============================================================================*/
int units_find_prefix (const char *s, int *skip)
  {
  // Note -- longest prefixes must test first

  if (strncmp (s, "tera", 4) == 0) { *skip = 4; return 12; }
  if (s[0] == 'T') { *skip = 1; return 12; }

  if (strncmp (s, "giga", 4) == 0) { *skip = 4; return 9; }
  if (s[0] == 'G') { *skip = 1; return 9; }

  if (strncmp (s, "mega", 4) == 0) { *skip = 4; return 6; }
  if (s[0] == 'M') { *skip = 1; return 6; }

  if (strncmp (s, "kilo", 4) == 0) { *skip = 4; return 3; }
  if (s[0] == 'k') { *skip = 1; return 3; }

  if (strncmp (s, "deci", 4) == 0) { *skip = 4; return -1; }
  if (s[0] == 'd') { *skip = 1; return -1; }

  if (strncmp (s, "nano", 4) == 0) { *skip = 4; return -9; }
  if (s[0] == 'n') { *skip = 1; return -9; }

  if (strncmp (s, "pico", 4) == 0) { *skip = 4; return -12; }
  if (s[0] == 'p') { *skip = 1; return -12; }

  if (strncmp (s, "centi", 5) == 0) { *skip = 5; return -2; }
  if (s[0] == 'c') { *skip = 1; return -2; }

  if (strncmp (s, "milli", 5) == 0) { *skip = 5; return -3; }
  if (strncmp (s, "micro", 5) == 0) { *skip = 5; return -6; }
  if (s[0] == 'm') { *skip = 1; return -3; }

  if (s[0] == 'u') { *skip = 1; return -6; }

  return 0;
  }


/*============================================================================
  unit_find_unit_by_name_and_prefix
============================================================================*/
Unit units_find_unit_by_name_and_prefix (const char *name, int *pref_pow, 
    BOOL allow_prefix)
  {
  if (allow_prefix)
    {
    int dummy;
    Unit u = units_find_unit_by_name_and_prefix (name, &dummy, FALSE);
    if ((int)u > 0)
      {
      pref_pow = 0;
      return u;
      } 
    }

  int skip;
  int test_pow;

  if (allow_prefix)
    {
    *pref_pow = 0;
    skip = 0;
    test_pow = units_find_prefix (name, &skip);
    }
  else
    {
    test_pow = 0;
    skip = 0;
    }

  if (test_pow != 0)
    {
    *pref_pow = test_pow;
    Unit t = units_find_unit_by_name (name + skip);
    if ((int)t > 0)
      return t; 
    else
      {
      *pref_pow = 0;
      return units_find_unit_by_name (name);
      }
    }
  return units_find_unit_by_name (name);
  }


/*============================================================================
  unit_parse_single_unit
  (Note -- we have to make special provision for unit names beginning with
  'cu' and 'sq', as these strings are also prefixes for 'cubic' and 'square')
============================================================================*/
void unit_parse_single_unit (const char *s, Unit *unit, int *power, 
    int *pref_power, char **error)
  {
  int i, ii = 0, l = strlen (s);
  char *ss = malloc (l + 1);

  // Remove ^
  for (i = 0; i < l; i++)
    {
    if (s[i] != '^')
      {
      ss[ii] = s[i];
      ii++;
      }
    }
  ss[ii] = 0;

  char temp[100];
  int ab_power = 0;
  int skip = 0;

  if (strstr(ss, "cubic ") == ss)
    {
    skip = 6;
    ab_power = 3;
    }
  else if (strstr(ss, "cubic") == ss)
    {
    skip = 5;
    ab_power = 3;
    }
  else if (strstr(ss, "cu ") == ss)
    {
    skip = 3;
    ab_power = 3;
    }
  else if (strstr(ss, "cu") == ss && strcasecmp(ss, "curie"))
    {
    skip = 2;
    ab_power = 3;
    }
  else if (strstr(ss, "square ") == ss)
    {
    skip = 7;
    ab_power = 2;
    }
  else if (strstr(ss, "square") == ss)
    {
    skip = 6;
    ab_power = 2;
    }
  else if (strstr(ss, "sq ") == ss)
    {
    skip = 3;
    ab_power = 2;
    }
  else if (strstr(ss, "sq") == ss)
    {
    skip = 2;
    ab_power = 2;
    }

  *power = ab_power;

  if (skip != 0)
    {
    strncpy (temp, ss, sizeof (temp) - 1);
    strcpy (ss, temp + skip);
    }

  int p = -1;
  l = strlen (ss);
  for (i = 0; i < l; i++)
    {
    char c = ss[i];
    if (c == '-' && isdigit((int)ss[i + 1]))
      {
      p = i;
      break;
      }
    if (isdigit((int)c))
      {
      p = i;
      break;
      }
    }
   
  char sunit[100];
  char spower[10];
  if (p >= 0)
    {
    if (ab_power == 0)
      {
      strncpy (spower, ss + p, sizeof (spower) - 1);
      ss[p] = 0;
      strncpy (sunit, ss, sizeof (sunit) - 1);
      }
    else
      *error = strdup ("Can't use prefix sq, cubic, etc., with an explicit power");
    }
  else
    {
    strncpy (sunit, ss, sizeof (sunit) - 1);
    strcpy (spower, "1");
    }

  if (ab_power == 0)
    *power = atoi (spower);
  if (*power == 0)
    {
    char msg[100];
    snprintf (msg, sizeof (msg), "Bad exponent: '%s'", spower);
    *error = strdup (msg); 
    }
  else
    {
    *pref_power = 0;
    *unit = units_find_unit_by_name_and_prefix (sunit, pref_power, TRUE);
    if ((int)(*unit) <= 0)
      {
      char msg[140];
      snprintf (msg, sizeof (msg), "Unknown unit name: '%s'", sunit);
      *error = strdup (msg); 
      }
    else
      {
      // We're done
      } 
    }

  //printf ("su=%s, pow=%s\n", sunit, spower);

  free (ss);
  }


/*============================================================================
  units_parse
============================================================================*/
Units *units_parse (const char *text, char **error)
  {
  Units *ret = malloc (sizeof (Units));
  ret->n_elements = 0;

  // Check for empty or null string -- this is valid: it's a zero-length unit list
  if (!text) return ret;
  if (text[0] == 0) return ret;

  int i = 0;

  const char *textp = text;
  BOOL div = FALSE;
  BOOL found = TRUE;

  if (text[0] == '/')
    {
    div = TRUE;
    textp++;
    }

  do
    {
    found = FALSE;
    BOOL ndiv = FALSE;
    const char *p = strchr (textp, '.');
    if (p)
      {
      ndiv = FALSE;
      found = TRUE;
      }
    else
      {
      p = strchr (textp, '/');
      if (p)
        {
        ndiv = TRUE;
        found = TRUE;
        }
      }
   
   char utemp[MAX_UNIT_STRING];

   if (p)
      {
      int ulen = p - textp; 
      if (ulen >= MAX_UNIT_STRING) ulen = MAX_UNIT_STRING - 1; // Should never happen
      strncpy (utemp, textp, ulen);
      utemp [ulen] = 0;
      }
   else
      {
      strcpy (utemp, textp);
      }
   
   // We've got a unit, and div is set if it's a dividing unit ('/sec')

  if (!*error)
    {
    // Successful parse of this unit
    Unit unit;
    int power;
    int pref_power;
    unit_parse_single_unit (utemp, &unit, &power, &pref_power, error);
    if (!*error)
      {
      ret->units[i].unit = unit;
      ret->units[i].prefix_power = pref_power;
      if (div) power = -power;
      ret->units[i].power = power;
      i++;
      }
    } 

   div = ndiv;
   textp = p + 1;
   } while (found && !*error && (i < MAX_UNIT_ELEMENTS)); 
   // TODO check MAX_UNIT_ELEMENTS overflow 

  ret->n_elements = i;

  if (*error)
    {
    units_free (ret);
    ret = NULL;
    }
 
  return ret;
  }


/*============================================================================
  units_find_conv_table_index
============================================================================*/
int units_find_conv_table_index (const Unit working_unit, int working_power)
  {
  int i = 0;
  while (conv_table[i].working_unit > 0)
    {
    if (conv_table[i].working_unit == working_unit && 
        conv_table[i].working_power == working_power) 
      return i;
    i++;
    }
  return -1;
  }


/*============================================================================
  units_find_unit_table_index
============================================================================*/
int units_find_unit_table_index (const Unit unit)
  {
  int i = 0;
  while (unit_table[i].unit > 0)
    {
    if (unit_table[i].unit == unit) return i;
    i++;
    }
  return -1;
  }


/*============================================================================
  units_get_name
============================================================================*/
const char *units_get_name (Unit unit, BOOL plural)
  {
  int index = units_find_unit_table_index (unit);
  if (index >= 0)
    {
    if (plural)
      return unit_table[index].plural_long_name;
    else
      return unit_table[index].long_name;
    }
  else
    return "?";
  }


/*============================================================================
  units_format_single_unit 
  // Used only by units_dump
============================================================================*/
char *units_format_single_unit (const UnitAndPower *unitAndPower)
  {
  const char *uname = units_get_name (unitAndPower->unit, FALSE);
  char *s = malloc (100); // TODO
  if (unitAndPower->prefix_power != 0)
    {
    const char *pref_name = units_format_prefix_name (unitAndPower->prefix_power);
    if (unitAndPower->power != 1)
      sprintf (s, "%s%s^%d", pref_name, uname, unitAndPower->power);
    else
      sprintf (s, "%s%s", pref_name, uname);
    }
  else
    {
    if (unitAndPower->power != 1)
      sprintf (s, "%s^%d", uname, unitAndPower->power);
    else
      sprintf (s, "%s", uname);
    }
  return s;
  }


/*============================================================================
  units_dump
============================================================================*/
void units_dump (const Units *self)
  {
  int i, l = self->n_elements;
  for (i = 0; i < l; i++)
    {
    char *s = units_format_single_unit (&self->units[i]); 
    printf ("%s", s);
    free (s);
    if (i != l - 1) printf (".");
    }
  printf ("\n");
  }


/*============================================================================
  units_free
============================================================================*/
void units_free (Units *self)
  {
  if (self)
    {
    free (self);
    }
  }


/*============================================================================
  units_compare_units
  Compare units, possibly accepting inverse units (km/h and h/km)
============================================================================*/
BOOL units_compare_units (const Units *u1, const Units *u2, BOOL allow_inverse,
     BOOL *inverse)
  {
  if (u1->n_elements != u2->n_elements) return FALSE;
  
  int i, j, l = u1->n_elements;

  BOOL seems_equal = FALSE;
  BOOL seems_inverse = FALSE;

  for (i = 0; i < l; i++)
    {
    BOOL found_equal = TRUE;
    BOOL found_inverse = TRUE;

    Unit try_unit = u1->units[i].unit;
    int try_power = u1->units[i].power;
    BOOL found = FALSE;
    for (j = 0; j < l && !found; j++)
      {
      Unit unit = u2->units[j].unit;
      int power = u2->units[j].power;
      if (unit == try_unit)
        { 
        if (power == try_power)
          {
          found_equal = TRUE;
          found_inverse = FALSE;
          found = TRUE;
          }
        else if (power == -try_power)
          {
          found_equal = FALSE;
          found_inverse = TRUE;
          found = TRUE;
          }
        else
          {
          found = FALSE;
          found_equal = FALSE;
          found_inverse = FALSE;
          }
        }
      else
        {
        found_equal = FALSE;
        found_inverse = FALSE;
        }
      }
    if (found_equal)
      {
      if (seems_inverse) return FALSE;
      seems_equal = TRUE; 
      }
    else if (found_inverse)
      {
      if (seems_equal) return FALSE;
      seems_inverse = TRUE; 
      }
    else
      return FALSE;
    }
  if (seems_equal) 
    {
    return TRUE;
    }
  if (seems_inverse) 
    {
    if (allow_inverse)
      {
      *inverse = TRUE;
      return TRUE;
      }
    }
  return FALSE;
  }


/*============================================================================
  units_convert_temp
============================================================================*/
double units_convert_temp (double n, Unit from, Unit to) 
  {
  if (from == to)
    return n;

  if (from == celsius && to == fahrenheit)
    return n * 1.8 + 32;
  if (from == celsius && to == kelvin)
    return n + 273.15;
  if (from == celsius && to == rankine)
    return n * 1.8 + 491.67;

  if (from == kelvin && to == fahrenheit)
    return (n - 273.15) * 1.8 + 32;
  if (from == kelvin && to == celsius)
    return n - 273.15;
  if (from == kelvin && to == rankine)
    return n * 1.8;

  if (from == fahrenheit && to == celsius)
    return (n - 32) * 5.0/9.0;
  if (from == fahrenheit && to == kelvin)
    return (n - 32) * 5.0/9.0 + 273.15;
  if (from == fahrenheit && to == rankine)
    return n + 459.67;

  if (from == rankine && to == fahrenheit)
    return n - 459.67;
  if (from == rankine && to == kelvin)
    return n * 5.0/9.0;
  if (from == rankine && to == celsius)
    return (n - 491.67) * 5.0/9.0;

  return 0; // We should never get here
  }


/*============================================================================
  units_insert_element
============================================================================*/
void units_insert_element (Units *units, Unit u, int power)
  {
  int i, l = units->n_elements;
  for (i = 0; i < l; i++)
    {
    if (units->units[i].unit == u)
      {
      // Unit already in set. Modify power
      units->units[i].power += power;

      // As a result of the insertion, we might have some elements with zero
      //  power. We must remove these, or it will mess up the unit comparison
      //  later

      Units temp_units;
      memset (&temp_units, 0, sizeof (temp_units));
      l = units->n_elements;
      for (i = 0; i < l; i++)
        {
        if (units->units[i].power != 0)
          {
          memcpy (&(temp_units.units[temp_units.n_elements]), &(units->units[i]),
             sizeof (UnitAndPower));
          temp_units.n_elements++;
          }
        }
      memcpy (units, &temp_units, sizeof (Units)); 
      return;
      }
    }

  if (units->n_elements == MAX_UNIT_ELEMENTS)
    {
    // This should never happen in normal operation; but we need to
    //  report it if it does
    fprintf (stderr, "Too many unit elements in units_insert_elements\n");
    exit (0);
    }
  
  // Unit does not exist -- add it
  units->units[l].unit = u;
  units->units[l].power = power;
  units->units[l].prefix_power = 0;
  units->n_elements++;
  }

/*============================================================================
  units_insert_elements
============================================================================*/
void units_insert_elements (Units *units, const Units *u, int power)
  {
  int i, l = u->n_elements;
  for (i = 0; i < l; i++)
    {
    units_insert_element (units, u->units[i].unit, u->units[i].power * power);
    } 
  }


/*============================================================================
  units_convert
============================================================================*/
double units_reduce_to_base_units (const Units *from_units, 
    Units *from_base_units, char **error)
  {
  double r = 1;
  from_base_units->n_elements = 0;
  int i, l = from_units->n_elements;
  BOOL is_rate = FALSE, has_temperature = FALSE;
  for (i = 0; i < l && !*error; i++)
    {
    int index = units_find_conv_table_index (from_units->units[i].unit, 1);
    if (index >= 0)
      {
      Units *base_units = &(conv_table[index].base_unit);
      r = r * pow (conv_table[index].slope * 
         pow (10, from_units->units[i].prefix_power), 
           from_units->units[i].power);
      units_insert_elements (from_base_units, base_units, from_units->units[i].power);

      if (from_units->units[i].power < 0)
        is_rate = TRUE;
      }
    else
      {
      Unit fu = from_units->units[i].unit;
      if (fu == celsius || fu == fahrenheit || fu == kelvin)
        {
        has_temperature = TRUE;
        }
      else
        {
        char s[256]; 
        snprintf (s, sizeof (s), "Internal error: No conversion defined for unit"
         " %d", from_units->units[i].unit);
        *error = strdup (s);
        }
      }      
    }

  if (has_temperature && !is_rate)
    {
    *error = strdup
      ("Units of temperature can only be converted to other units of temperature "
       "if they are not part of a rate");
    }

  return r;
  }


/*============================================================================
  temperature_unit
============================================================================*/
BOOL temperature_unit (const Units *u)
  {
    switch (u->units[0].unit)
      {
      case celsius:
      case fahrenheit:
      case kelvin:
      case rankine:
        return u->n_elements == 1;
      default:
        return FALSE;
      }
  }


/*============================================================================
  units_convert
============================================================================*/
double units_convert (double n, const Units *from_units, 
    const Units *to_units, char **error)
  {
  // Check for temperature conversion, which is a special case
  if (temperature_unit (from_units) && temperature_unit (to_units))
    return units_convert_temp (n, from_units->units[0].unit, to_units->units[0].unit);

  // Not temperature. Check general cases

  Units from_base_units;
  double from_factor = units_reduce_to_base_units (from_units, &from_base_units, 
    error);
  if (!*error)
    {
    Units to_base_units;
    double to_factor = units_reduce_to_base_units (to_units, &to_base_units, 
      error);
    if (!*error)
      {
      BOOL inverse = FALSE;
      if (units_compare_units (&from_base_units, &to_base_units, TRUE, &inverse))
        {
        if (inverse)
          {
          double factor = from_factor*to_factor;
          double res = n * factor;
          return 1.0/res;
          }
        else 
          {
          double factor = from_factor/to_factor;
          double res = n * factor;
          return res;
          }
        }
      else
        {
        char s[256];
        char *ss1 = units_format_string (from_units, FALSE); 
        char *ss2 = units_format_string (to_units, FALSE); 
        snprintf (s, sizeof (s), 
          "Can't convert %s to %s,\nbecause their base dimensions are different", 
         ss1, ss2);
        free (ss1);
        free (ss2);
        //units_dump (&from_base_units);
        //units_dump (&to_base_units);
        *error = strdup (s); 
        }
      }
    }

  return 0; // Get here only on error
  }


/*============================================================================
  units_format_string
  plural = render the plural form of the name, if there is one
============================================================================*/
char *units_format_string (const Units *self, BOOL plural)
  {
  char s[256];
  s[0] = 0;

  int i, last_numerator = -1, l = self->n_elements;
  for (i = 0; i < l; i++)
   {
   if (self->units[i].power < 0)
     break;
   last_numerator = i;
   }

  for (i = 0; i < l; i++)
    {
    int power = self->units[i].power;
    int prefix_power = self->units[i].prefix_power;
    Unit unit = self->units[i].unit;

    // Note that we only pluralize the last unit of the numerator. It is
    // feet/second, not feet/seconds and Newton.metres, not Newtons.metre.
    const char *uname = units_get_name (unit, plural && (i == last_numerator));

    if (i == 0 && power == -1)
      {
      if (power == -1) strcat (s, "/"); // TODO
      }
    else if (i != 0)
      {
      if (power == -1) 
        strcat (s, "/"); // TODO
      else if (power < -1 && power >= -3)
        strcat (s, "/");
      else
        strcat (s, ".");
      }

    if (prefix_power != 0)
     {
     const char *pref_name = units_format_prefix_name (prefix_power);
     strcat (s, pref_name); // TODO
     }

    if (power == 2 || power == -2)
      strcat (s, "square ");
   
    if (power == 3 || power == -3)
      strcat (s, "cubic ");

    strcat (s, uname); // TODO

    if (power == -1)
      {
      // Do nothing -- we took care of this already
      }
    else if (power == 1)
      {
      // Do nothing -- ^1 never needs to be displayed
      }
    else
      {
      if (power > 3 || power < -3)
        sprintf (s + strlen (s), "^%d", power); // TODO 
      }
    }
  char *ret = strdup (s);
  return ret;
  }


/*============================================================================
  units_format_feet
============================================================================*/
char *units_format_feet (double feet)
  {
  char s[64];
  s[0] = 0;

  if (feet < 0) 
    {
    feet = -feet;
    strcpy (s, "-");
    }
    
  double whole_feet = floor (feet);
  double inches = (feet - whole_feet) * 12.0;

  if (inches != 0.00)
    {
#ifdef WIN32
    sprintf (s + strlen (s), 
      "%d %s, %lG %s", (int)whole_feet, whole_feet == 1.000 ? "foot" : "feet",
        inches, inches == 1.000 ? "inch" : "inches");
#else
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s, %lG %s", (int)whole_feet, whole_feet == 1.000 ? "foot" : "feet",
        inches, inches == 1.000 ? "inch" : "inches");
#endif
    }
  else
    {
#ifdef WIN32
    sprintf (s + strlen (s), 
      "%d %s", (int)whole_feet, whole_feet == 1.000 ? "foot" : "feet");
#else
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s", (int)whole_feet, whole_feet == 1.000 ? "foot" : "feet");
#endif
    }

  return strdup(s);
  }


/*============================================================================
  units_format_yards
============================================================================*/
char *units_format_yards (double yards)
  {
  char s[64];
  s[0] = 0;

  if (yards < 0) 
    {
    yards = -yards;
    strcpy (s, "-");
    }
    
  double whole_yards = floor (yards);
  double feet = (yards - whole_yards) * 3.0;


  if (feet != 0.00)
    {
    char *s_feet = units_format_feet (feet);
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s, %s", (int)whole_yards, 
        whole_yards == 1.000 ? "yard" : "yards", s_feet);
    free (s_feet);
    }
  else
    {
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s", (int)whole_yards, 
        whole_yards == 1.000 ? "yard" : "yards");
    }
  
  return strdup(s);
  }


/*============================================================================
  units_format_miles
============================================================================*/
char *units_format_miles (double miles)
  {
  char s[64];
  s[0] = 0;

  if (mile < 0) 
    {
    miles = -miles;
    strcpy (s, "-");
    }
    
  double whole_miles = floor (miles);
  double yards = (miles - whole_miles) * 1760.0;


  if (yards != 0.00)
    {
    char *s_yards = units_format_yards (yards);
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s, %s", (int)whole_miles, 
        whole_miles == 1.000 ? "mile" : "miles", s_yards);
    free (s_yards);
    }
  else
    {
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s", (int)whole_miles, 
        whole_miles == 1.000 ? "mile" : "miles");
    }
  
  return strdup(s);
  }


/*============================================================================
  units_format_minutes
============================================================================*/
char *units_format_minutes (double minutes)
  {
  char s[64];
  s[0] = 0;

  if (minutes < 0) 
    {
    minutes = -minutes;
    strcpy (s, "-");
    }
    
  double whole_minutes = floor (minutes);
  double seconds = (minutes - whole_minutes) * 60.0;

  if (seconds != 0.00)
    {
#ifdef WIN32
    sprintf (s + strlen (s), 
      "%d %s, %lG %s", (int)whole_minutes, 
        whole_minutes == 1.000 ? "minute" : "minutes",
        seconds, seconds == 1.000 ? "second" : "seconds");
#else
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s, %lG %s", (int)whole_minutes, 
        whole_minutes == 1.000 ? "minute" : "minutes",
        seconds, seconds == 1.000 ? "second" : "seconds");
#endif
    }
  else
    {
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s", (int)whole_minutes, whole_minutes == 1.000 ? "minute" : "minutes");
    }

  return strdup(s);
  }


/*============================================================================
  units_format_hours
============================================================================*/
char *units_format_hours (double hours)
  {
  char s[64];
  s[0] = 0;

  if (hour < 0) 
    {
    hours = -hours;
    strcpy (s, "-");
    }
    
  double whole_hours = floor (hours);
  double minutes = (hours - whole_hours) * 60.0;

  if (minutes != 0.00)
    {
    char *s_minutes = units_format_minutes (minutes);
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s, %s", (int)whole_hours, 
        whole_hours == 1.000 ? "hour" : "hours", s_minutes);
    free (s_minutes);
    }
  else
    {
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s", (int)whole_hours, 
        whole_hours == 1.000 ? "hour" : "hours");
    }
  
  return strdup(s);
  }


/*============================================================================
  units_format_pounds
============================================================================*/
char *units_format_pounds (double pounds)
  {
  char s[64];
  s[0] = 0;

  if (pounds < 0) 
    {
    pounds = -pounds;
    strcpy (s, "-");
    }
    
  double whole_pounds = floor (pounds);
  double ounces = (pounds - whole_pounds) * 16.0;

  if (ounces != 0.00)
    {
#ifdef WIN32
    sprintf (s + strlen (s), 
      "%d %s, %lG %s", (int)whole_pounds, 
        whole_pounds == 1.000 ? "pound" : "pounds",
        ounces, ounces == 1.000 ? "ounce" : "ounces");
#else
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s, %lG %s", (int)whole_pounds, 
        whole_pounds == 1.000 ? "pound" : "pounds",
        ounces, ounces == 1.000 ? "ounce" : "ounces");
#endif
    }
  else
    {
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s", (int)whole_pounds, whole_pounds == 1.000 ? "pound" : "pounds");
    }

  return strdup(s);
  }


/*============================================================================
  units_format_stones
============================================================================*/
char *units_format_stones (double stones)
  {
  char s[64];
  s[0] = 0;

  if (stone < 0) 
    {
    stones = -stones;
    strcpy (s, "-");
    }
    
  double whole_stones = floor (stones);
  double pound = (stones - whole_stones) * 14.0;

  if (pound != 0.00)
    {
    char *s_pound = units_format_pounds (pound);
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s, %s", (int)whole_stones, 
        whole_stones == 1.000 ? "stone" : "stones", s_pound);
    free (s_pound);
    }
  else
    {
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s", (int)whole_stones, 
        whole_stones == 1.000 ? "stone" : "stones");
    }
  
  return strdup(s);
  }


/*============================================================================
  units_format_hundredweight
============================================================================*/
char *units_format_hundredweight (double hundredweight)
  {
  char s[64];
  s[0] = 0;

  if (hour < 0) 
    {
    hundredweight = -hundredweight;
    strcpy (s, "-");
    }
    
  double whole_hundredweight = floor (hundredweight);
  double stone = (hundredweight - whole_hundredweight) * 8.0;

  if (stone != 0.00)
    {
    char *s_stone = units_format_stones (stone);
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s, %s", (int)whole_hundredweight, 
        whole_hundredweight == 1.000 ? "hundredweight" : "hundredweight", s_stone);
    free (s_stone);
    }
  else
    {
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s", (int)whole_hundredweight, 
        whole_hundredweight == 1.000 ? "hundredweight" : "hundredweight");
    }
  
  return strdup(s);
  }


/*============================================================================
  units_format_tons
============================================================================*/
char *units_format_tons (double tons)
  {
  char s[64];
  s[0] = 0;

  if (ton < 0) 
    {
    tons = -tons;
    strcpy (s, "-");
    }
    
  double whole_tons = floor (tons);
  double hundredweight = (tons - whole_tons) * 20.0;

  if (hundredweight != 0.00)
    {
    char *s_hundredweight = units_format_hundredweight (hundredweight);
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s, %s", (int)whole_tons, 
        whole_tons == 1.000 ? "ton" : "tons", s_hundredweight);
    free (s_hundredweight);
    }
  else
    {
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s", (int)whole_tons, 
        whole_tons == 1.000 ? "ton" : "tons");
    }
  
  return strdup(s);
  }


/*============================================================================
  units_format_pints
============================================================================*/
char *units_format_pints (double pints)
  {
  char s[64];
  s[0] = 0;

  if (pints < 0) 
    {
    pints = -pints;
    strcpy (s, "-");
    }
    
  double whole_pints = floor (pints);
  double fluid_ounces = (pints - whole_pints) * 16.0;

  if (fluid_ounces != 0.00)
    {
#ifdef WIN32
    sprintf (s + strlen (s), 
      "%d %s, %lG %s", (int)whole_pints, 
        whole_pints == 1.000 ? "pint" : "pints",
        fluid_ounces, fluid_ounces == 1.000 ? "fluid-ounce" : "fluid-ounces");
#else
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s, %lG %s", (int)whole_pints, 
        whole_pints == 1.000 ? "pint" : "pints",
        fluid_ounces, fluid_ounces == 1.000 ? "fluid-ounce" : "fluid-ounces");
#endif
    }
  else
    {
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s", (int)whole_pints, whole_pints == 1.000 ? "pint" : "pints");
    }

  return strdup(s);
  }


/*============================================================================
  units_format_gallons
============================================================================*/
char *units_format_gallons (double gallons)
  {
  char s[64];
  s[0] = 0;

  if (gallon < 0) 
    {
    gallons = -gallons;
    strcpy (s, "-");
    }
    
  double whole_gallons = floor (gallons);
  double pint = (gallons - whole_gallons) * 8.0;

  if (pint != 0.00)
    {
    char *s_pint = units_format_pints (pint);
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s, %s", (int)whole_gallons, 
        whole_gallons == 1.000 ? "gallon" : "gallons", s_pint);
    free (s_pint);
    }
  else
    {
    snprintf (s + strlen (s), sizeof (s) - strlen(s), 
      "%d %s", (int)whole_gallons, 
        whole_gallons == 1.000 ? "gallon" : "gallons");
    }
  
  return strdup(s);
  }


/*============================================================================
  units_format_string_and_value
============================================================================*/
char *units_format_string_and_value (const Units *self, double n, 
    BOOL force_decimal)
  {
  BOOL dummy;

  if (!force_decimal)
    {
    if (units_compare_units (self, &units_mile, FALSE, &dummy))
      return units_format_miles (n);

    if (units_compare_units (self, &units_yard, FALSE, &dummy))
      return units_format_yards (n);

    if (units_compare_units (self, &units_foot, FALSE, &dummy))
      return units_format_feet (n);

    if (units_compare_units (self, &units_hour, FALSE, &dummy))
      return units_format_hours (n);

    if (units_compare_units (self, &units_minute, FALSE, &dummy))
      return units_format_minutes (n);

    if (units_compare_units (self, &units_ton, FALSE, &dummy))
      return units_format_tons (n);

    if (units_compare_units (self, &units_hundredweight, FALSE, &dummy))
      return units_format_hundredweight (n);

    if (units_compare_units (self, &units_pound, FALSE, &dummy))
      return units_format_pounds (n);

    if (units_compare_units (self, &units_stone, FALSE, &dummy))
      return units_format_stones (n);

    if (units_compare_units (self, &units_gallon, FALSE, &dummy))
      return units_format_gallons (n);

    if (units_compare_units (self, &units_pint, FALSE, &dummy))
      return units_format_pints (n);
    }


  char s[256];
  char *s_unit = units_format_string (self, n != 1.000);
#ifdef WIN32
  sprintf (s, "%lG %s", n, s_unit);
#else
  snprintf (s, sizeof (s), "%lG %s", n, s_unit);
#endif
  free (s_unit);
  return strdup (s);
  }


/*============================================================================
  units_dump_tables
============================================================================*/
void units_dump_tables (FILE *f)
  {
  int i = 0;
  fprintf (f, "%-15s %-21s %s\n", "Name", "Description", "Synonyms"); 

  while (unit_table[i].unit > 0)
    {
    fprintf (f, "%-15s %-21s %s\n", unit_table[i].long_name, unit_table[i].desc, 
      unit_table[i].alt_names);
    i++;
    }
  
  printf ("\n");
  printf ("Units can be used in combination: m/sec, lumen/sqinch, J.sec/kg etc\n");
  }



