:Author: jeffs1212
:Email: jeffrey_stevens@sbcglobal.net
:Date: 13/02/2018
:Revision: version#
:License: Public Domain

= Project: {sprinkler_system}

Description:
  home sprinkler system - 6 zone control the sprinkler system 
  includes; 
    Adafruit Metro Serial 
    LCD 16 x2 display 
    PCF8523 RTC 
    8-way rotory switch 
    2 - momentary pushbutton switches 
    6 relay module

  The 8-way switch is used to select zones 1-6, All, or None 
  The "enable" momentary switch selects or unselects the selected valve(s) for the zone(s) determined by the 8-way switch 
  The "test" momentary switch starts or stops the selected valve running a fixed duration test cycle of the zone

  The LCD displays the time and date, the selected zones and when a zone is running the time remaining
  
  12/20/18 starting the sprinkler run_time based on RTC = water day and start_time still needs to be written

== Step 1: Installation
Please describe the steps to install this project.

For example:

1. Open this file
2. Edit as you like
3. Release to the World!

== Step 2: Assemble the circuit

Assemble the circuit following the diagram layout.png attached to the sketch

== Step 3: Load the code

Upload the code contained in this sketch on to your board

=== Folder structure

....
 sketch123                => Arduino sketch folder
  ├── sketch123.ino       => main Arduino file
  ├── schematics.png      => (optional) an image of the required schematics
  ├── layout.png          => (optional) an image of the layout
  └── ReadMe.adoc         => this file
....

=== License
This project is released under a {License} License.

=== Contributing
To contribute to this project please contact jeffs1212 <jeffrey_stevens@sbcglobal.net>

=== BOM
Add the bill of the materials you need for this project.

|===
| ID | Part name      | Part number | Quantity
| R1 | 10k Resistor   | 1234-abcd   | 10       
| L1 | Red LED        | 2345-asdf   | 5        
| A1 | Arduino Zero   | ABX00066    | 1        
|===


=== Help
This document is written in the _AsciiDoc_ format, a markup language to describe documents. 
If you need help you can search the http://www.methods.co.nz/asciidoc[AsciiDoc homepage]
or consult the http://powerman.name/doc/asciidoc[AsciiDoc cheatsheet]
