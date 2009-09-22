#!/usr/bin/env python

# Create an Measurement Set Antenna table using the values
# from a SCHED location file.
# Stephen Bourke, JIVE.
# Sept, 2009.
#
# Version: 20090914

import pyrap_tables
from pyrap_tables import tablecreatescalarcoldesc as cldsc
from pyrap_tables import tablecreatearraycoldesc as clarrdsc
import re
import sys
import os.path

prog_dir = os.path.dirname(__file__)
if prog_dir == '':
	prog_dir = '.'
# original file in ftp://ftp.aoc.nrao.edu/pub/sched/sched_8.1/sched_8.1.tar.gz
location_file = prog_dir + '/locationsSB2.dat'

# Define some arrays for convienience
EVN = ['EB','JB','MC','NT','ON','SG','TR','UR','WB']
EEVN = ['EB','CM','JB2','MC','ON','TR','WB']
MERLIN6 = ['CM', 'JB2', 'DA', 'DE', 'KN', 'TA']
MERLIN7 = ['CM', 'JB2', 'JB', 'DA', 'DE', 'KN','TA']
VLBA = ['BR', 'FD','HN','KP','LA','MK','NL','OV','PT','SC']
VLAA = ['YN8','YN16','YN24','YN32','YN40','YN48','YN56','YN64','YN72',
        'YE8','YE16','YE24','YE32','YE40','YE48','YE56','YE64','YE72',
        'YW8','YW16','YW24','YW32','YW40','YW48','YW56','YW64','YW72']
VLAB = ['YN4','YN8','YN12','YN16','YN20','YN24','YN28','YN32','YN36',
        'YE4','YE8','YE12','YE16','YE20','YE24','YE28','YE32','YE36',
        'YW4','YW8','YW12','YW16','YW20','YW24','YW28','YW32','YW36']
VLAC = ['YN2','YN4','YN6','YN8','YN10','YN12','YN14','YN16','YN18',
        'YE2','YE4','YE6','YE8','YE10','YE12','YE14','YE16','YE18',
        'YW2','YW4','YW6','YW8','YW10','YW12','YW14','YW16','YW18']
VLAD = ['YN1','YN2','YN3','YN4','YN5','YN6','YN7','YN8','YN9',
        'YE1','YE2','YE3','YE4','YE5','YE6','YE7','YE8','YE9',
        'YW1','YW2','YW3','YW4','YW5','YW6','YW7','YW8','YW9']
HSA = ['AR','EB','GB', 'Y'] + VLBA
GLOBAL = EVN + VLBA
GMVA = ['EB','ON','MH','PV','NL','FD','LA','KP','PT','OV','BR','MK'] # PB?
WSRT = ['WRT0','WRT1','WRT2','WRT3','WRT4',
        'WRT5','WRT6','WRT7','WRT8','WRT9',
        'WRTA','WRTB','WRTC','WRTD']
WSRT10 = ['WRT0','WRT1','WRT2','WRT3','WRT4',
          'WRT5','WRT6','WRT7','WRT8','WRT9']

LOFAR_CORE_LBA = ['CS002LBA', 'CS003LBA', 'CS004LBA', 'CS005LBA',
                 'CS006LBA', 'CS007LBA', 'CS021LBA', 'CS024LBA',
                 'CS030LBA', 'CS032LBA', 'CS103LBA', 'CS201LBA',
                 'CS301LBA', 'CS302LBA', 'CS401LBA', 'CS501LBA']
LOFAR_CORE_HBA = ['CS002HBA0', 'CS002HBA1', 'CS003HBA0', 'CS003HBA1',
                 'CS004HBA0', 'CS004HBA1', 'CS005HBA0', 'CS005HBA1',
                 'CS006HBA0', 'CS006HBA1', 'CS007HBA0', 'CS007HBA1',
                 'CS021HBA0', 'CS021HBA1', 'CS024HBA0', 'CS024HBA1',
                 'CS030HBA0', 'CS030HBA1', 'CS032HBA0', 'CS032HBA1',
                 'CS103HBA0', 'CS103HBA1', 'CS201HBA0', 'CS201HBA1',
                 'CS301HBA0', 'CS301HBA1', 'CS302HBA0', 'CS302HBA1',
                 'CS401HBA0', 'CS401HBA1', 'CS501HBA0', 'CS501HBA1']
LOFAR_REMOTE_LBA = ['RS106LBA', 'RS208LBA', 'RS306LBA', 'RS307LBA',
                   'RS503LBA', 'RS509LBA']
LOFAR_REMOTE_HBA = ['RS106HBA', 'RS208HBA', 'RS306HBA', 'RS307HBA',
                   'RS503HBA', 'RS509HBA']
LOFAR_INTERNATIONAL_LBA = ['DE601LBA', 'DE602LBA', 'DE603LBA']
LOFAR_INTERNATIONAL_HBA = ['DE602HBA']
LOFAR_CORE = LOFAR_CORE_LBA + LOFAR_CORE_HBA
LOFAR_REMOTE = LOFAR_REMOTE_LBA + LOFAR_REMOTE_HBA
LOFAR_INTERNATIONAL = LOFAR_INTERNATIONAL_LBA + LOFAR_INTERNATIONAL_HBA
LOFAR_NL_LBA = LOFAR_CORE_LBA + LOFAR_REMOTE_LBA
LOFAR_NL_HBA = LOFAR_CORE_HBA + LOFAR_REMOTE_HBA
LOFAR_NL = LOFAR_CORE + LOFAR_REMOTE
ELOFAR_LBA = LOFAR_CORE_LBA + LOFAR_REMOTE_LBA + LOFAR_INTERNATIONAL_LBA
ELOFAR_HBA = LOFAR_CORE_HBA + LOFAR_REMOTE_HBA + LOFAR_INTERNATIONAL_HBA
ELOFAR = LOFAR_CORE + LOFAR_REMOTE + LOFAR_INTERNATIONAL

# Antenna size defaults to 25m, override here
ANT_SIZE = {'EB': 100, 'JB': 76, 'MC': 32, 'NT':32, 'ON': 20, 'TR': 32,
            'MH': 14, 'YB': 40, 'AR': 305, 'WZ': 20, 'RO': 70, 'DS65': 34,
            'KB': 34, 'MA': 20, 'NY': 20, 'GT': 100, 'JB2': 32, 'CA': 32}
            # WB, Y ?

# Sizes of lofar stations
# http://www.astron.nl/radio-observatory/astronomers/users/technical-information/lofar-stations/lofar-stations-description-
def lofar_size(name):
	if (name.startswith('CS') or name.startswith('RS')) and name.endswith('LBA'):
		# Core and Remote LBA
		return 87
	elif name.endswith('LBA'):
		# International LBA
		return 70
	elif name.endswith('HBA0') or name.endswith('HBA1'):
		# Core HBA
		return 35
	elif name.startswith('RS') and name.endswith('HBA'):
		# Remote HBA
		return 50.2
	elif name.endswith('HBA'):
		# International HBA
		return 62
	else:
		raise ValueError, "%s doesn't appear to be a lofar ant" % name

# Use an ordered dictionary so table columns are in the expected order
class ord_dict(dict):
    def __init__(self):
        dict.__init__(self)
        self._order = []
    def __setitem__(self, key, value):
        self._order.append(key)
        dict.__setitem__(self, key, value)
    def keys(self):
        return self._order
    def __iter__(self):
        self._iter_n = 0
        return self
    def next(self):
        if self._iter_n == len(self._order):
            raise StopIteration
        self._iter_n += 1
        return self._order[self._iter_n - 1]

# From pyrap but using ordered dictionary
def tablecreatedesc(descs=[]):
    rec = ord_dict()
    for desc in descs:
        colname = desc['name']
        if rec.has_key(colname):
            raise ValueError('Column name ' + name + ' multiply used in table description')
        rec[colname] = desc['desc']
    return rec;

# REs for parsing SCHED locations file
comment_line = re.compile('^!')
name_line = re.compile("\s*DBCODE='?\s*(\w+)'?\s*DBNAME='?([\w-]+)'?\s*AXISTYPE='(\w+)'\s*AXISOFF=\s*(-?\d+\.\d+)\s*")
xyz_line = re.compile('\s*X=\s*(-?\d+\.\d+)\s*Y=\s*(-?\d+\.\d+)\s*Z=\s*(-?\d+\.\d+)\s*')
end_block = re.compile('^/')

if len(sys.argv) != 3:
	print >> sys.stderr, 'Usage: %s <out_table> "<antennas>"' % sys.argv[0]
	sys.exit()

ms_out = sys.argv[1]
try:
	exec('user_antennas = '+ sys.argv[2])
except SyntaxError:
	print >> sys.stderr, 'Antenna list not valid: "%s"' % sys.argv[2]
	print >> sys.stderr, "eg: \"['EB', 'JB', 'MC', 'NT', 'TR']\""
	print >> sys.stderr, "    \"EVN + VLBA + ['AR', 'Y']\""
	print >> sys.stderr, "    \"LOFAR_REMOTE_LBA + ['CS401LBA', 'CS501LBA']\""
	sys.exit()

# Get antenna parms from file
ant_list = []
have_name = False
have_xyz = False
try:
	inf = open(location_file)
except IOError:
	print >> sys.stderr, 'Could not open antenna locations file', location_file
	sys.exit()
for line in inf:
	if comment_line.match(line):
		continue
	name_match = name_line.match(line)
	if name_match:
		have_name = True
		code, name, mount, offset = name_match.groups()
		# TODO: use the offset and DT values too?
		if mount == 'alta' or mount == 'altz' or mount == 'altaz':
			mount = 'ALT-AZ'
		elif mount == 'equa':
			mount = 'EQUATORIAL'
		elif mount == 'spac':
			mount = 'SPACE-HALCA'
		elif mount == 'xyEW' or mount == 'xyNS':
			mount = 'X-Y'
		else:
			print >> sys.stderr, 'Unknown mount %s on %s using alt-az' % (mount, code)
			mount = 'ALT-AZ'
		#print 'matched', code, name
	xyz_match = xyz_line.match(line)
	if xyz_match:
		have_xyz = True
		x, y, z = [float(i) for i in xyz_match.groups()]
	if end_block.match(line):
		assert have_name and have_xyz
		have_name = False
		have_xyz = False
		if code in [ant['NAME'] for ant in ant_list]:
			print >> sys.stderr, 'Warning: Duplicate entries for "%s" found in locations file. Using first found.' % code
		elif code in user_antennas:
			ant_parms = {'POSITION': [x,y,z], 'MOUNT': mount,
				'STATION': name, 'NAME': code}
			if code in ANT_SIZE:
				ant_parms['DISH_DIAMETER'] = float(ANT_SIZE[code])
			elif name == 'LOFAR':
				ant_parms['DISH_DIAMETER'] = lofar_size(code)
			ant_list.append(ant_parms)
inf.close()

# Check if we found all antennas in the file
codes = [ant['NAME'] for ant in ant_list]
for ant in user_antennas:
	if ant not in codes:
		print >> sys.stderr, 'Warning: Antenna "%s" not found' % ant

# Define columns
offset_desc = clarrdsc('OFFSET', value=float(), ndim=1, shape=[3])
position_desc = clarrdsc('POSITION', value=float(), ndim=1, shape=[3])
type_desc = cldsc('TYPE', value=str())
dish_desc = cldsc('DISH_DIAMETER', value=float())
flag_desc = cldsc('FLAG_ROW', value=bool())
mount_desc = cldsc('MOUNT', value=str())
name_desc = cldsc('NAME', value=str())
station_desc = cldsc('STATION', value=str())

desc = tablecreatedesc([offset_desc, position_desc, type_desc,
        dish_desc, flag_desc, mount_desc, name_desc, station_desc])

# Create and populate our table
table = pyrap_tables.table(ms_out, desc, nrow=len(ant_list), readonly=False)
table.putcolkeywords('OFFSET', {'QuantumUnits': ['m', 'm', 'm'],
    'MEASINFO': {'Ref': 'ITRF', 'type': 'position'}})
table.putcolkeywords('POSITION', {'QuantumUnits': ['m', 'm', 'm'],
    'MEASINFO': {'Ref': 'ITRF', 'type': 'position'}})
table.putcolkeywords('DISH_DIAMETER', {'QuantumUnits': ['m']})
table[:] = {'DISH_DIAMETER': 25.0, 'OFFSET': [0.0,0.0,0.0], 'TYPE': 'GROUND-BASED'}
for i in range(len(ant_list)):
	table[i] = ant_list[i]
