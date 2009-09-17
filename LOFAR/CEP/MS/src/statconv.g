# Script to store station positions to an AIPS++ ANTENNA table
# with the positions in ITRF (m,m,m).

include 'measures.g'
include 'quanta.g'
include 'table.g'

doit := function()
{
    t:=table('LOFAR_ANTENNA',readonly=F);
    filec:=open('< lofar_stations.coord');
    read(filec);     #skip header
    nrl:=0;
    while (line := read(filec)) {
	nrl+:=1;
	vals := split(line,';');
	lat  := as_double(vals[2]);
	long := as_double(vals[3]);
	pos := dm.position('wgs84', dq.unit(long,'deg'),
			   dq.unit(lat,'deg'), dq.unit(0.,'m'));
	name := spaste('S', vals[1] ~ s/["]//g);    #"; add quote for emacs
	# Convert to ITRF and to units m,m,m.
        posi := dm.addxvalue (dm.measure (pos, 'itrf'));
	posvec := [posi[1].value, posi[2].value, posi[3].value];
	print name, posvec;
	t.putcell('NAME', nrl, name);
	t.putcell('POSITION', nrl, posvec);
	units := [posi[1].unit, posi[2].unit, posi[3].unit];
    }
    t.putcolkeyword ('POSITION', 'QuantumUnits', units);

    clat := 6.87001 * pi / 180;          # core center latitude
    clon := 52.91548 * pi / 180;         # core center longitude
    cpos := dm.position('wgs84', dq.unit(clon,'rad'),
			dq.unit(clat,'rad'), dq.unit(0.,'m'));
    # Convert to ITRF and to units m,m,m.
    cposi := dm.addxvalue (dm.measure (cpos, 'itrf'));
    cposvec := [cposi[1].value, cposi[2].value, cposi[3].value];
    # Define radius and nr of stations per circle.
    rads := [180,290,470,750];          # radius from core center
    nrs  := [5,8,8,11];
    # Define stations in a circle around the core center with given radius.
    # Do it with evenly distributed angles. Start at some angle.
    dlat := pi/2 / 1e7;                    # radians per m in lat direction
    dlon := pi/2 / (cos(clat) * 1e7);      # radians per m in lon direction
    for (i in [1:len(rads)]) {
	nr := nrs[i];
	anglediff := 2*pi/nr;
	angle     := (rads[i] % 97) * pi / 180;
	for (j in [1:nr]) {
	    nrl+:=1;
	    lon := clon + rads[i] * sin(angle) * dlon;
	    lat := clat + rads[i] * cos(angle) * dlat;
	    pos := dm.position('wgs84', dq.unit(lon,'rad'),
			       dq.unit(lat,'rad'), dq.unit(0.,'m'));
	    name := spaste('C', i, '.', j);
	    # Convert to ITRF and to units m,m,m.
	    posi := dm.addxvalue (dm.measure (pos, 'itrf'));
	    posvec := [posi[1].value, posi[2].value, posi[3].value];
	    t.putcell('NAME', nrl, name);
	    t.putcell('POSITION', nrl, posvec);
	    angle +:= anglediff;
	    dposvec := posvec - cposvec;
	    print name,posvec,' dist:',sqrt(sum(dposvec*dposvec));
	}
    }
    t.close();
    print nrl;
}
