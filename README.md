marnav
======

[![Build Status](https://travis-ci.org/mariokonrad/marnav.svg?branch=master)](https://travis-ci.org/mariokonrad/marnav)

Copyright (c) 2015 Mario Konrad (mario.konrad@gmx.net)

---

Abstract
--------

This is a C++ library for **MAR**itime **NAV**igation purposes.

It supports (partially):
- NMEA-0183
- AIS
- SeaTalk (Raymarine device communication)
- Reading data from serial ports (NMEA, SeaTalk)
- Basic geodesic functions, suitable for martime navigation.

See chapter _Features_ for a complete and detailed list.

---

Goals
-----

There are already implementaions for reading and writing NMEA-0183 or AIS.
The goal of this library is
- to have an implementation in modern C++ (C++11 right now, C++14 as soon as compilers
  catch up, but not necessarily bleeding edge)
- easy to use API (std lib like, integrating well)
- unit tested (high test coverage)
- trivial integration into projects (liberal license, as few dependencies as
  possible)
- (more or less) well documented
- (more or less) complete, as far as information is freely available
- having fun

---

Features
--------

### NMEA-0183

Supported sentences for NMEA-0183 (read and write):
- AAM: Waypoint Arrival Alarm
- ALM: GPS Almanac Data
- APB: Autopilot Sentence "B"
- BOD: Bearing - Waypoint to Waypoint
- BWC: Bearing & Distance to Waypoint - Geat Circle
- BWR: Bearing and Distance to Waypoint - Rhumb Line
- BWW: Bearing - Waypoint to Waypoint
- DBT: Depth Below Transducer
- DPT: Depth of Water
- DSC: Digital Selective Calling Information (**experimental**)
- DSE: Extended DSC (**experimental**)
- DTM: Datum Reference
- FSI: Frequency Set Information
- GBS: GPS Satellite Fault Detection
- GGA: Global Positioning System Fix Data
- GLC: Geographic Position, Loran-C
- GLL: Geographic Position - Latitude/Longitude
- GNS: Fix data
- GRS: GPS Range Residuals
- GSA: Geographic Position - Latitude/Longitude
- GST: GPS Pseudorange Noise Statistics
- GSV: Satellites in view
- GTD: Geographic Location in Time Differences
- HDG: Heading - Deviation & Variation
- HFB: Trawl Headrope to Footrope and Bottom
- HSC: Heading Steering Command
- ITS: Trawl Door Spread 2 Distance
- LCD: Loran-C Signal Data
- MSK: Control for a Beacon Receiver
- MSS: Beacon Receiver Status
- MTW: Mean Temperature of Water
- MWD: Wind Direction and Speed
- MWV: Wind Speed and Angle
- OSD: Own Ship Data
- RMA: Recommended Minimum Navigation Information
- RMB: Recommended Minimum Navigation Information
- RMC: Recommended Minimum Navigation Information
- ROT: Rate Of Turn
- RPM: Revolutions
- RSA: Rudder Sensor Angle
- RSD: RADAR System Data (**experimental**)
- RTE: Routes
- SFI: Scanning Frequency Information
- TDS: Trawl Door Spread Distance
- TFI: Trawl Filling Indicator
- TLL: Target Latitude and Longitude
- TPC: Trawl Position Cartesian Coordinates
- TPR: Trawl Position Relative Vessel
- TPT: Trawl Position True
- TTM: Tracked Target Message
- VBW: Dual Ground/Water Speed
- VDM: AIS VHF Data-Link Message
- VDO: AIS VHF Data-Link Own-Vessel Report
- VDR: Set and Drift
- VHW: Water speed and heading
- VLW: Distance Traveled through Water
- VTG: Track made good and Ground speed
- VWR: Relative Wind Speed and Angle.
- VPW: Speed - Measured Parallel to Wind
- WCV: Waypoint Closure Velocity
- WNC: Distance - Waypoint to Waypoint
- WPL: Waypoint Location
- XDR: Transducer Measurement
- XTE: Cross-Track Error, Measured
- XTR: Cross Track Error - Dead Reckoning
- ZDA: Time & Date - UTC, day, month, year and local time zone
- ZDL: Time and Distance to Variable Point
- ZFO: UTC & Time from origin Waypoint
- ZTG: UTC & Time to Destination Waypoint

Obsolete (according to the standard) but implemented:
- DBK: Depth Below Keel
- HDM: Heading - Magnetic (obsolete as of 2009)
- R00: Waypoints in active route

Vendor Extensions:
- PGRME: Garmin Estimated Error


### AIS

Supported messages for AIS (decode and encode):
- Type 01: Position Report Class A
- Type 02: Position Report Class A (Assigned Schedule)
- Type 03: Position Report Class A (Response to Interrogation)
- Type 04: Base Station Report
- Type 05: Static and Voyage Related Data
- Type 09: Standard SAR Aircraft Position Report
- Type 10: UTC/Date Inquiry
- Type 11: UTC/Date Response
- Type 18: Standard Class B CS Position Report
- Type 19: Extended Class B CS Position Report
- Type 21: Aid-to-Navigation Report
- Type 24: Static Data Report (part A and B, norma and auxiliary vessel)


### SeaTalk

Suported messages for SeaTalk (decode and encode):
- Type 00: depth below transducer
- Type 01: equipment id
- Type 05: Engine RPM and PITCH
- Type 10: apparent wind angle
- Type 11: apparent wind speed
- Type 20: speed through water
- Type 21: trip mileage
- Type 22: total mileage
- Type 23: water temperature 1
- Type 24: Display unit for Mileage and Speed
- Type 25: total and trip log
- Type 26: Speed through Water
- Type 27: water temperature 2
- Type 30: Set Lamp Intensity
- Type 36: Cancel MOB condition
- Type 38: Codelock data (**experimental**)
- Type 50: LAT Postion
- Type 51: LON Postion


### IO

- Reading data from serial ports (NMEA, SeaTalk)


### Geodesic Functions

Basic geodesic functions, suitable for martime navigation.

- Calculation of CPA (closest point of approach)
  and TCPA (time to closest point of approach)
- Distance of two points on a sphere
- Distance of two points on an ellipsoid using formula of Vincenty
- Distance of two points on an ellipsoid using formula of Lambert


---

Examples
--------

More examples [here](doc/examples.md).

### Parse NMEA Sentence

~~~~~~~~~~~~~{.cpp}
using namespace marnav;

auto sentence = nmea::make_sentence(
	"$GPRMC,201034,A,4702.4040,N,00818.3281,E,0.0,328.4,260807,0.6,E,A*17");
std::cout << sentence->tag() << "\n";
auto rmc = nmea::sentence_cast<nmea::rmc>(sentence);
std::cout << "latitude : " << nmea::to_string(rmc->get_lat()) << "\n";
std::cout << "longitude: " << nmea::to_string(rmc->get_lon()) << "\n";
~~~~~~~~~~~~~

### Write NMEA Sentence

~~~~~~~~~~~~~{.cpp}
nmea::mtw mtw;
mtw.set_temperature(22.5);
std::string data = nmea::to_string(mtw);
~~~~~~~~~~~~~

### Parse AIS Message from NMEA data

~~~~~~~~~~~~~{.cpp}
using namespace marnav;

// received sentences
const std::vector<std::string> received_strings
	= {"!AIVDM,2,1,3,B,55P5TL01VIaAL@7WKO@mBplU@<PDhh000000001S;AJ::4A80?4i@E53,0*3E",
		"!AIVDM,2,2,3,B,1@0000000000000,2*55"};

// parse NMEA sentences
std::vector<std::unique_ptr<nmea::sentence>> sentences;
for (auto const & txt : received_strings) {
	auto sentence = nmea::make_sentence(txt);
	if (sentence->id() == nmea::sentence_id::VDM) {
		sentences.push_back(std::move(sentence));
	}
}

// parse and and process AIS messags
auto payload = nmea::collect_payload(sentences.begin(), sentences.end());
auto message = ais::make_message(payload);
if (message->type() == ais::message_id::static_and_voyage_related_data) {
	auto report = ais::message_cast<ais::message_05>(message);
	std::cout << "shipname: " << report->get_shipname() << "\n";
	std::cout << "callsign: " << report->get_callsign() << "\n";
}
~~~~~~~~~~~~~

### Create NMEA sentences from AIS data

~~~~~~~~~~~~~{.cpp}
using namespace marnav;

// prepare AIS data
ais::message_01 pos_report;
pos_report.set_sog(82);
// ... most data not shown here

// create payload
auto payload = ais::encode_message(pos_report);

// create NMEA sentences
for (uint32_t fragment = 0; fragment < payload.size(); ++fragment) {
	nmea::vdm vdm;
	vdm.set_n_fragments(payload.size());
	vdm.set_fragment(fragment + 1);
	vdm.set_radio_channel(nmea::ais_channel::B);
	vdm.set_payload(payload[fragment]);

	// collect, send or do something with the sentence...
	std::string s = nmea::to_string(vdm);
	std::cout << s << "\n";
}
~~~~~~~~~~~~~

---

Requirements
------------

This chapter describes the requirements in order to build the library.
Tools and their versions are listed below, newer/older/other tools
(i.e. compilers, etc.) may work, but not tested.

Compiler:
- GCC 4.9.2
- GCC 5.1.0
- Clang 3.6
- Clang 3.7

Tools needed to build the library:
- cmake 3.0
- GNU make 3.81

Tools needed to develop the library:
- git
- clang-format 3.7

Tools needed to build the documentation:
- doxygen
- graphviz
- LaTeX (there are formulas!)

Optional used for development:
- lcov / genhtml, c++filt
- cppcheck
- clang-analyze (3.7 or newer)
- ctags, cscope

Optional (no core dependency):
- Boost.ASIO (used only for some examples)
- Qt 5 (used only for some examples)

Opeating system:
- Linux

There are no other dependencies despite the standard library (C++11)
to build this library.

---

Build
-----

The following build types `-DCMAKE_BUILD_TYPE=x` are possible:
- Debug
- Release
- Coverage

Build options:
- `ENABLE_STATIC` : enables static build, if `OFF`, a shared library is being built.
  Default: `ON`
- `ENABLE_WARNING_HELL` : enables _much_ more warnings, used for development purposes.
  Currently implemented only for GCC.  Default is `OFF`
- `ENABLE_PROFILING` : enables profiling


### Library

	mkdir build
	cd build
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make


### Documentation

	mkdir build
	cd build
	cmake ..
	make doc


### Package

	make package

or individual package types:

	cpack -G TGZ
	cpack -G DEB


### Developpers Choice

	mkdir build
	cd build
	cmake -DCMAKE_BUILD_TYPE=Coverage -DENABLE_WARNING_HELL=YES ..
	make -j 8
	make coverage doc cppcheck


### Static Analysis with Clang

	mkdir build
	cd build
	cmake -DCMAKE_CXX_COMPILER=/usr/share/clang/scan-build-3.7/c++-analyzer ..
	scan-build-3.7 --use-analyzer=/usr/bin/clang++-3.7 make

After the build, ```scan-build``` will tell you what to do in order to inspect
the findings.


---

Authors
-------

Mario Konrad (mario.konrad@gmx.net) with help from others.

Search the repository for a complete list:

	git log --format=%an | sort -u

---

Links
-----

A (non-complete) collection of resources from where information was gathered.

- [NMEA Revealed](http://www.catb.org/gpsd/NMEA.html) (by Eric S. Raymond)
- [NMEA FAQ](http://www.kh-gps.de/nmea.faq)
- [it-digin's blog](http://www.it-digin.com/blog/?cat=4)
- [AIVDM/AIVDO Protocol decoding](http://www.catb.org/gpsd/AIVDM.html) (by Eric S. Raymond)
- [DSC Position Request](http://www.thehulltruth.com/marine-electronics-forum/43945-dsc-position-request.html)
- [NMEA-0183 Sentences DSC,DSE](http://www.cruisersforum.com/forums/f13/nmea-0183-sentences-dsc-dse-124887.html)
- [SerialMon - NMEA 0183 Protocol](http://www.serialmon.com/protocols/nmea0183.html)
- [SeaTalk Reference](http://thomasknauf.de/seatalk.htm) (by Thomas Knauf)
- [Navigation Center - AIS Standard Class B Equipment Position Report](http://www.navcen.uscg.gov/?pageName=AISMessagesB)
- [GPS Forums](http://www.gps-forums.net)
- [NMEA Datensaetze](http://www.nmea.de/nmea0183datensaetze.html)

---

LICENSE
-------

> NOTE:
> The official NMEA 0183 Standard document is not available for free. It was not
> consulted at any point during the development of this library. All information
> was found from free sources on the internet. This library (especially the NMEA
> part) is not derivative work of this standard.


See also [LICENSE](LICENSE)

(BSD)

```
Copyright (c) 2015, Mario Konrad
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by Mario Konrad.
4. Neither the name of the software nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```


