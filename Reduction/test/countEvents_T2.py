#! /usr/bin/env python

import os, sys, logging, re
from optparse import OptionParser
from xml.dom.minidom import parseString


usage = "usage: python %prog T2_list_of_files.list directoryCrab/res [options]"
parser = OptionParser(usage=usage)
parser.add_option("--verbose",action="store_true", help="verbose option (default=False)", default=False)

(options, args) = parser.parse_args()
if len(args)<1 :
  parser.print_help()
  sys.exit(1)

path=args[0]
pathfjr=args[1] 

#if not os.path.exists(path):
#  print 'directory '+path+' does not exist. Exiting.'
#  sys.exit(1)

jobs=[]

dirList = open(path, 'r')
for line in dirList:
  if '.root' in line:
    filename = line.rstrip('\n')
    splitfilename = filename.split('_')
    jobnumber=splitfilename[len(splitfilename)-3]
    jobs.append(jobnumber)

grandTotalPassed = 0
grandTotalProcessed = 0

for job in jobs:
  if options.verbose:
    print '**********processing job '+str(job) 
  fjrname = pathfjr+'/crab_fjr_'+job+'.xml'
  try :
    fjr = open(fjrname, 'r')
  except Exception,e:
    print 'Error: %s' %str(e)
  #convert to string
  data=fjr.read()
  fjr.close()
  dom = parseString(data)
  totaleventsTags = dom.getElementsByTagName('TotalEvents')
  if len(totaleventsTags) != 1:
    print 'problem with totaleventsTags. Found '+str(len(totaleventsTags))+' tags instead of 1. Skipping job '+str(job)
    continue
  totaleventsTag = totaleventsTags[0].toxml()
  totaleventsData=totaleventsTag.replace('<TotalEvents>','').replace('</TotalEvents>','')
  
  totalevents = totaleventsData.rstrip('\n')
  grandTotalPassed = grandTotalPassed + float(totalevents)
  if options.verbose:
    print 'number of events passed for job '+str(job)+': '+str(float(totalevents))

  eventreadTags = dom.getElementsByTagName('EventsRead')
  eventsreadjob=0
  if options.verbose:
    print 'this job run on '+str(len(eventreadTags))+' files'
  for tag in eventreadTags:
    eventreadTag=tag.toxml()
    eventreadData=eventreadTag.replace('<EventsRead>','').replace('</EventsRead>','')
    eventread=float(eventreadData)
    if options.verbose:
      print '\tProcessed events '+str(eventread)
    eventsreadjob=eventsreadjob+eventread
  
  grandTotalProcessed = grandTotalProcessed+eventsreadjob
  if options.verbose:
    print 'total processed events for job '+str(job)+': '+str(eventsreadjob)

print '>>>>>>>>>> Total processed events:\t'+str(grandTotalProcessed)
print '>>>>>>>>>> Total events skimmed:\t'+str(grandTotalPassed)
print '>>>>>>>>>> Skimming efficiency:\t'+str(grandTotalPassed/grandTotalProcessed)



#print jobs
