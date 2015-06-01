#!/usr/bin/env python

from operator import itemgetter
import sys

current_year = None
current_origin = None

current_count = 0
current_partial_sum = 0

year = None
origin = None

for line in sys.stdin:
    line = line.strip()

    origin, data = line.split(',')
    year, partial_sum, partial_count = data.split('\t')

    try:
        partial_sum = int(partial_sum)
        partial_count = int(partial_count)
    except ValueError:
        continue

    if current_year == year and current_origin == origin:
        current_partial_sum += partial_sum
        current_count += partial_count
    else:
        if current_year and current_origin:
            mean = float(current_partial_sum) / float(current_count)
            print current_year, current_origin, mean

        current_year = year
        current_origin = origin
        current_partial_sum = partial_sum
        current_count = partial_count

if current_year and current_origin and current_year == year and current_origin == origin:
    mean = float(current_partial_sum) / float(current_count)
    print current_year, current_origin, mean
