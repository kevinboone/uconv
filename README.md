<h1>uconv -- a general unit converter for the command line</h1> 

Version 0.0.4, June 2024 

<h2>What is this?</h2>

<code>uconv</code> is a general-purpose unit converter for use on the 
command line. It is primarily intended for domestic and general 
scientific applications, supporting conversions for length, area, volume,
mass, force, energy, radiation dose, fuel economy, temperature, pressure,
 velocity, acceleration, data capacity, and many others. For non-metric units,
<code>uconv</code> favours UK variants over US ones, but the US versions are
also available. 

<h2>Why another unit converter?</h2>

There are many (perhaps too many) unit converter programs available; most have
graphical interfaces. <code>uconv</code> is intended to be sufficiently
flexible in its unit specification that it is faster to use than a graphical
program. 
<p/>
An example of command line usage might be:

<pre class="codeblock">
$ uconv 60 cuft m3
60 cubic feet = 1.69901 cubic metres
</pre>

A more complex example:

<pre class="codeblock">
$ uconv 200 m/sec2/g ft/min2/lb
200 metres/square second/gramme = 1.07148E+09 feet/square minute/pound
</pre>

The number and the first unit can be written together:

<pre class="codeblock">
$ uconv 2gib/sec mb/sec
2 gibibytes/second = 2147.48 megabytes/second
</pre>

<code>uconv</code> recognizes a hundred or so basic units, but the potential
conversions are very much more extensive than this, as units can be combined.
The program recognizes combined units in a variety of formats:
m/sec, m/sec^2, m/sec2, m/sec/sec, etc. 
<p/>
In fact, <code>uconv</code> will convert between any pair of units that are
dimensionally consistent. For example, you can convert joules per second per
kilogram to calories per minute per pound, if you wish. The program won't
attempt to convert, for example, minutes to metres -- these are not
dimensionally consistent units. However, any pair of measurements that 
can be reduced
to the same basic set of core units (metre, gramme, second, newton, 
ampere, byte)
should be convertible. <code>uconv</code> will even convert units that
are complementary, that is, having dimensions that are the inverse of
one another. So it can convert, for example, minutes per mile into miles per hour. 
<p/>
<code>uconv</code> attempts to be very flexible in the way units are
interpreted, so the user doesn't have to spend too much time studying a manual
to find the proper format. So square meters, for example, can be entered as 'sq
m', 'square meter', 'sq metres', 'm2', 'sqm', among many others. The usual SI
prefixes -- kilo, micro, etc -- are recognized, both in full and as
abbreviations.
<p/>
As of version 0.0.4, <code>uconv</code> includes data capacity among
its basic unit, and can convert quantities involving both metric and
IEC capacity units. That is, it can convert quanities involving
(IEC) gibibytes and the like, to quanitities involving (metric) megabytes 
and the like. 
<code>uconv</code> is written in ANSI-standard C, and will compile on
more-or-less any platform with a C compiler.
<p/>
Although there are many unit conversion applications around, I wrote
<code>uconv</code> because I needed something that would work on the command line,
on all the platforms I use, including Android. This means, essentially,
a plain C program. The GNU 'units' utility almost fits the bill 
-- units is hugely flexible but, for me, way too 
clunky in its use of the command line. In addition, its bloated
database of obscure units makes it hard to find things when you're not sure
of the name or abbreviation.
<p/>
Please note: <code>uconv</code> is not related in any way to the Unicode
converter with the same name.

<h2>Building and installation</h2>

To build from source, unpack the source bundle and 
run <code>make</code>, followed
by <code>make install</code>. <code>uconv</code> has no unusual dependencies,
and should build on most POSIX-like platforms.

<h2>Further information</h2>

See the [uconv man page](uconv.man.html).

<h2>Legal and copying</h2>

<code>uconv</code> is maintained by Kevin Boone with substantial 
contributions from other authors. It is copyright (c)2013-2024, distributed
under the terms of the GNU Public Licence, version 3.0. There is no
warranty of any kind.


