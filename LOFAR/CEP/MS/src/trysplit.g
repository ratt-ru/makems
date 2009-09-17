dosplit := function(ant1, ant2, nparts)
{
    # Fill matrix telling which baselines are available.
    #  0 = not available
    #  1 = available
    # -1 = available mirrored (thus 1 in opposite cell)
    nrbl := len(ant1);
    nrant := max(max(ant1), max(ant2));
    ##print nrant,ant1,ant2
    blmat := array(0, nrant, nrant);
    for (i in [1:nrbl]) {
	if (blmat[ant1[i],ant2[i]] != 0) {
	    fail spaste('baseline ',ant1[i],'-',ant2[i],' multiple defined');
	}
	blmat[ant1[i],ant2[i]] := 1;         # real baseline
	if (ant1[i] != ant2[i]) {
	    blmat[ant2[i],ant1[i]] := -1;    # mirrored baseline
	}
    }
    # Determine nr of baselines per part (evenly divided).
    ##print nrbl,nparts
    nrblpart := as_integer(nrbl / nparts);
    nfpart := nrbl - nrblpart*nparts;         # nr of full parts
    # Loop through all baselines and try to group them such that each group
    # contains the fewest nr of antennas.
    # First find the width of a square containing antennas giving a group.
    width := 1;
    ##print nrant,width,nrblpart,nfpart
    a1 := [1];
    a2 := [nrant];
    while (sum(abs(blmat[a1,a2])) < nrblpart) {
	a2 := [(nrant-width):nrant];
	width +:= 1;
	a1 := [1:width];
    }
    ##print sum(abs(blmat[a1,a2]))
    ##print nrant,width,nrblpart,nfpart
    # Now start grouping all baselines.
    # Iterate through the matrix with squares of the width found above.
    # If at the end of the 2nd axis, reverse the order and increase axis1.
    # So it zigzags through the matrix.
    # Note that iterating always uses 0..nrant. The direction
    # determines the actual index for the 2nd axis.
    st1 := 1;     # start axis1 at beginning
    st2 := 1;     # start axis2 at beginning
    dir2 := 1;    # start axis2 in up direction
    width1 := width;   # initially full width
    width2 := width;
    for (partnr in [1:nparts]) {
	nrpbl := nrblpart;
	if (partnr <= nfpart) nrpbl +:= 1;    # #baselines needed in this group
	##print 'nrpbl=',nrpbl;
	nrb := 0;                             # #baselines found so far
	while (T) {
	    # Each time take an extra slice of the square.
	    # So it first looks in a [1,1] square up to the full square width.
	    # It is hoped that in this way as few antennas as possible are used
	    for (i2 in [0:(width2-1)]) {
		inx2 := st2+i2;	
		if (dir2 < 0) {
		    inx2 := nrant+1 - inx2;
		}
		for (i1 in [0:(width1-1)]) {
		    inx1 := st1+i1;
		    ##print 'inx=',inx1,inx2;
		    if (abs(blmat[inx1,inx2]) == 1) {
			blmat[inx1,inx2] *:= partnr+1;
			if (inx2 != inx1) {
			    blmat[inx2,inx1] *:= partnr+1;
			}
			##print 'found',inx1,inx2;
			nrb +:= 1;
			if (nrb == nrpbl) break;
		    }
		}
		if (nrb == nrpbl) break;
	    }
	    if (nrb == nrpbl) break;
	    nst2 := st2+width2;
	    if (nst2 > nrant) {
		dir2 := -dir2;      # swap direction
		st1 +:= width;
		if (st1 > nrant){
		    ##print blmat;
		    fail 'st1>nrant should never occur';
		}
		width1 := min(width, nrant+1-st1);
		st2 := 1;
	    } else {
		st2 := nst2;
		width2 := min(width, nrant+1-st2);
	    }
	}
	##print 'nrb=',nrb;
	##print blmat;
    }
    return blmat-1;
}

dosplit2 := function(ant1, ant2, nparts)
{
    # Fill matrix telling which baselines are available.
    #  0 = not available
    #  1 = available
    # -1 = available mirrored (thus 1 in opposite cell)
    nrbl := len(ant1);
    nrant := max(max(ant1), max(ant2));
    ##print nrant,ant1,ant2
    blmat := array(0, nrant, nrant);
    for (i in [1:nrbl]) {
	if (blmat[ant1[i],ant2[i]] != 0) {
	    fail spaste('baseline ',ant1[i],'-',ant2[i],' multiple defined');
	}
	blmat[ant1[i],ant2[i]] := 1;         # real baseline
	if (ant1[i] != ant2[i]) {
	    blmat[ant2[i],ant1[i]] := -1;    # mirrored baseline
	}
    }
    # Determine nr of baselines per part (evenly divided).
    ##print nrbl,nparts
    nrblpart := as_integer(nrbl / nparts);
    nfpart := nrbl - nrblpart*nparts;         # nr of full parts
    # Loop through all baselines and try to group them such that each group
    # contains the fewest nr of antennas.
    # First find the width of a square containing antennas giving a group.
    width := 1;
    ##print nrant,width,nrblpart,nfpart
    a1 := [1];
    a2 := [nrant];
    while (sum(abs(blmat[a1,a2])) < nrblpart) {
	a2 := [(nrant-width):nrant];
	width +:= 1;
	a1 := [1:width];
    }
    ##print sum(abs(blmat[a1,a2]))
    ##print nrant,width,nrblpart,nfpart
    # Now start grouping all baselines.
    # Iterate through the matrix with squares of the width found above.
    # If at the end of the 2nd axis, reverse the order and increase axis1.
    # So it zigzags through the matrix.
    # Note that iterating always uses 0..nrant. The direction
    # determines the actual index for the 2nd axis.
    st1 := 1;     # start axis1 at beginning
    st2 := 1;     # start axis2 at beginning
    dir2 := 1;    # start axis2 in up direction
    width1 := width;   # initially full width
    width2 := width;
    for (partnr in [1:nparts]) {
	nrpbl := nrblpart;
	if (partnr <= nfpart) nrpbl +:= 1;    # #baselines needed in this group
	##print 'nrpbl=',nrpbl;
	nrb := 0;                             # #baselines found so far
	while (T) {
	    # Each time take an extra slice of the square.
	    # So it first looks in a [1,1] square up to the full square width.
	    # It is hoped that in this way as few antennas as possible are used
	    for (step in [0:(width-1)]) {
		##print 'sw1w2=',step,width1,width2,st1,st2,dir2;
		if (step < width1) {
		    inx1 := st1+step;
		    if (inx1 <= nrant) {
			for (i in [0:min(step,width2-1)]) {
			    i2 := st2+i;
			    if (dir2 < 0) {
				i2 := nrant+1 - i2;
			    }
			    ##print 'i2=',inx1,i2;
			    if (abs(blmat[inx1,i2]) == 1) {
				blmat[inx1,i2] *:= partnr+1;
				if (i2 != inx1) {
				    blmat[i2,inx1] *:= partnr+1;
				}
				##print 'found',inx1,i2;
				nrb +:= 1;
				if (nrb == nrpbl) break;
			    }
			}
		    }
		    if (nrb == nrpbl) break;
		}
		if (step < width2) {
		    inx2 := st2+step;
		    if (inx2 <= nrant) {
			if (dir2 < 0) {
			    inx2 := nrant+1 - inx2;
			}
			for (i in [0:min(step,width1-1)]) {
			    i1 := st1+i;
			    ##print 'i1=',i1,inx2;
			    if (abs(blmat[i1,inx2]) == 1) {
				blmat[i1,inx2] *:= partnr+1;
				if (i1 != inx2) {
				    blmat[inx2,i1] *:= partnr+1;
				}
				##print 'found',i1,inx2;
				nrb +:= 1;
				if (nrb == nrpbl) break;
			    }
			}
		    }
		    if (nrb == nrpbl) break;
		}
	    }
	    if (nrb == nrpbl) break;
	    nst2 := st2+width2;
	    if (nst2 > nrant) {
		dir2 := -dir2;      # swap direction
		st1 +:= width;
		if (st1 > nrant){
		    ##print blmat;
		    fail 'st1>nrant should never occur';
		}
		width1 := min(width, nrant+1-st1);
		st2 := 1;
	    } else {
		st2 := nst2;
		width2 := min(width, nrant+1-st2);
	    }
	}
	##print 'nrb=',nrb;
	##print blmat;
    }
    return blmat-1;
}

findant := function(mat, grp)
{
    ant := [];
    for (i in [1:(shape(mat)[1])]) {
	if (any(mat[i,]==grp) || any(mat[,i]==grp)) {
	    ant:=[ant,i];
	}
    }
    return ant;
}

findhist := function(mat)
{
    nparts := max(mat);
    nra := shape(mat)[1];
    histg := array(0,nra);
    for (i in [1:nparts]) {
	ant := findant(mat,i);
	nrbl := sum(mat[mat==i]) / i;
	print 'part',i,'has',nrbl,'baselines and',len(ant),'antennas',ant;
	histg[len(ant)] +:= 1;
    }
    return histg;
}

trysplit := function(nra, nparts, autocorr=T)
{
    nrbl := nra*(nra-1)/2;
    if (autocorr) {
	nrbl +:= nra;
    }
    ant1:=array(0,nrbl);
    ant2:=array(0,nrbl);
    i := 1;
    for (a1 in [1:nra]) {
	for (a2 in [a1:nra])  {
	    if (autocorr  ||  a1 != a2) {
		ant1[i] := a1;
		ant2[i] := a2;
		i +:= 1;
	    }
	}
    }
    mat1 := dosplit (ant1, ant2, nparts);
    hist1 := findhist(mat1);
    nra1 := [1:nra][hist1>0];
    last1 := nra1[len(nra1)];
    freq1 := hist1[hist1>0];
    print 'histogram1 #ant',nra1;
    print '           freq',freq1;
    mat2 := dosplit2 (ant1, ant2, nparts);
    hist2 := findhist(mat2);
    nra2 := [1:nra][hist2>0];
    last2 := nra2[len(nra2)];
    freq2 := hist2[hist2>0];
    print 'histogram2 #ant',nra2;
    print '           freq',freq2;
    print 'Use mat1?',last1<last2 ||
	              (last1==last2 && hist1[last1] < hist2[last2]);
    return T;
}
