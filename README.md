# adaptive-server
A lightweight web server written in C that adapts files (only images) for every different machine.
It uses libWURFL to retrieve device capabilities relative to the User-Agent, then manipulates images according to these infiormations.
(currenty under development)

# TO-DO
- Add hashmap for files cache
- Implement Pre-Threading
- Add IPv6 support
- Different policy for log file writing (maybe another thread/process that add logs to queue and wirtes to file whene necessary)

# Usage
- Install dependencies
- Run 'install.sh' inside development folder
- Run 'adaptiveserver'
- Drink a beer with your firends :)

# Dependencies
- libWURFL (https://github.com/filosganga/libwurfl)
- ImageMagick (http://www.imagemagick.org/script/magick-wand.php)

# License

This program is copyright 2016 Andrea Silvi.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the `GNU General Public License`_ for more details.
