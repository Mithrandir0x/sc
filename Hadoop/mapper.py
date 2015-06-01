#!/usr/bin/env python

import sys

current_year = None
current_origin = None
current_count = 0

sum_delays = 0

year = None
origin = None

for line in sys.stdin:
    line = line.strip()
    words = line.split(',')

    year = words[0]
    delay = words[15]
    origin = words[16]
    cancelled = words[21]

    try:
        current_delay = int(delay)
        cancelled = int(cancelled)
    except ValueError:
        continue

    if cancelled == 0 and origin:
        if current_year == year and current_origin == origin:
            current_count += 1
            sum_delays += current_delay
        else:
            if current_year and current_origin:
                print  '%s,%s\t%s\t%s' % (current_origin, current_year, sum_delays, current_count)

            sum_delays = current_delay
            current_year = year
            current_origin = origin
            current_count = 1

if current_year and current_origin and current_year == year and current_origin == origin:
    print  '%s,%s\t%s\t%s' % (current_origin, current_year, sum_delays, current_count)
