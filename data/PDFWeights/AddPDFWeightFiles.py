#!/usr/bin/env python

import sys, glob

if len(sys.argv) != 3:
   print "Usage: %s <root file pattern with full path> <txt output file>"
   sys.exit(1)

pattern = sys.argv[1]
out = open(sys.argv[2], 'w')
l = glob.glob(pattern)
print "Found %d files matching pattern" % len(l)
l.sort()

n_file = 0
sum_nevt = 0
sum_weights = []
for filename in l:
   #print filename
   infile = open(filename, 'r')   
   content = infile.readlines()
   n_line = 0
   
   for line in content:
      numbers = line.split()
      #print numbers
      #print len(numbers)
      if n_line == 0:
         sum_nevt += int(numbers[0])

      elif n_line == 1:
         if n_file == 0:
            for entry in numbers:
               sum_weights.append(float(entry))

            print 'Number of PDF weights: %d' % len(sum_weights)

         else:
            n_entry = 0
            for entry in numbers:
               sum_weights[n_entry] += float(entry)
               n_entry += 1
            ###
         ###
      ###
      n_line += 1
   ###
   n_file += 1
   infile.close()
##


out.write(str(sum_nevt)+'\n')
for i in range(0, len(sum_weights)):
   entry = sum_weights[i]
   out.write(str(entry))
   if i != len(sum_weights)-1:
      out.write(' ')

out.close()

#for file in l:
#    out.write('<In FileName="%s" Lumi="0.0"/>\n' % file)
#out.close()
