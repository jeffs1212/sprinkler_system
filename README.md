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

  4/2/20  Merged last code used from online Arduino IDE editor.  Renamed IDE file to "sprinkler - final"
          There are some includes at the top of the file that may not be necessary.
          
          

