# See LICENSE file for copyright and license details.

BEGIN {
	FS = ";"
	# set up hexadecimal lookup table
	for(i = 0; i < 16; i++)
		hex[sprintf("%X",i)] = i;
	HEADER = "/* Automatically generated by mkrunetype.awk */\n#include <stdlib.h>\n\n#include \"utf.h\"\n#include \"runetype.h\"\n"
}

$3  ~ /^L/ { alphav[alphac++] = $1; }
($3  ~ /^Z/) || ($5 == "WS") || ($5 == "S") || ($5 == "B") { spacev[spacec++] = $1; }
$3 == "Cc" { cntrlv[cntrlc++] = $1; }
$3 == "Lu" { upperv[upperc++] = $1; tolowerv[uppercc++] = ($14 == "") ? $1 : $14; }
$3 == "Ll" { lowerv[lowerc++] = $1; toupperv[lowercc++] = ($13 == "") ? $1 : $13; }
$3 == "Lt" { titlev[titlec++] = $1; }
$3 == "Nd" { digitv[digitc++] = $1; }

END {
	mkis("alpha", alphav, alphac, "src/isalpharune.c", q, "");
	mkis("space", spacev, spacec, "src/isspacerune.c", q, "");
	mkis("cntrl", cntrlv, cntrlc, "src/iscntrlrune.c", q, "");
	mkis("upper", upperv, upperc, "src/upperrune.c",   tolowerv, "lower");
	mkis("lower", lowerv, lowerc, "src/lowerrune.c",   toupperv, "upper");
	mkis("title", titlev, titlec, "src/istitlerune.c", q, "");
	mkis("digit", digitv, digitc, "src/isdigitrune.c", q, "");
}

# parse hexadecimal rune index to int
function code(s) {
	x = 0;
	for(i = 1; i <= length(s); i++) {
		c = substr(s, i, 1);
		x = (x*16) + hex[c];
	}
	return x;
}

# generate 'is<name>rune' unicode lookup function
function mkis(name, runev, runec, file, casev, casename) {
	rune1c = 0;
	rune2c = 0;
	rune3c = 0;
	rune4c = 0;
	mode = 1;

	#sort rune groups into singletons, ranges and laces
	for(j = 0; j < runec; j++) {
		# range
		if(code(runev[j+1]) == code(runev[j])+1 && ((length(casev) == 0) ||
		   code(casev[j+1]) == code(casev[j])+1) && j+1 < runec) {
			if (mode == 2) {
				continue;
			} else if (mode == 3) {
				rune3v1[rune3c] = runev[j];
				rune3c++;
			} else if (mode == 4) {
				rune4v1[rune4c] = runev[j];
				rune4c++;
			}
			mode = 2;
			rune2v0[rune2c] = runev[j];
			if(length(casev) > 0) {
				case2v[rune2c] = casev[j];
			}
			continue;
		}
		# lace 1
		if(code(runev[j+1]) == code(runev[j])+2 && ((length(casev) == 0) ||
		   (code(casev[j+1]) == code(runev[j+1])+1 && code(casev[j]) == code(runev[j])+1)) &&
		   j+1 < runec) {
			if (mode == 3) {
				continue;
			} else if (mode == 2) {
				rune2v1[rune2c] = runev[j];
				rune2c++;
			} else if (mode == 4) {
				rune4v1[rune2c] = runev[j];
				rune4c++;
			}
			mode = 3;
			rune3v0[rune3c] = runev[j];
			continue;
		}
		# lace 2
		if(code(runev[j+1]) == code(runev[j])+2 && ((length(casev) == 0) ||
		   (code(casev[j+1]) == code(runev[j+1])-1 && code(casev[j]) == code(runev[j])-1)) &&
		   j+1 < runec) {
			if (mode == 4) {
				continue;
			} else if (mode == 2) {
				rune2v1[rune2c] = runev[j];
				rune2c++;
			} else if (mode == 3) {
				rune3v1[rune2c] = runev[j];
				rune3c++;
			}
			mode = 4;
			rune4v0[rune4c] = runev[j];
			continue;
		}
		# terminating case
		if (mode == 1) {
			rune1v[rune1c] = runev[j];
			if (length(casev) > 0) {
				case1v[rune1c] = casev[j];
			}
			rune1c++;
		} else if (mode == 2) {
			rune2v1[rune2c] = runev[j];
			rune2c++;
		} else if (mode == 3) {
			rune3v1[rune3c] = runev[j];
			rune3c++;
		} else { #lace 2
			rune4v1[rune4c] = runev[j];
			rune4c++;
		}
		mode = 1;
	}
	print HEADER > file;

	#generate list of laces 1
	if(rune3c > 0) {
		print "static Rune "name"3[][2] = {" > file;
		for(j = 0; j < rune3c; j++) {
			print "\t{ 0x"rune3v0[j]", 0x"rune3v1[j]" }," > file;
		}
		print "};\n" > file;
	}

	#generate list of laces 2
	if(rune4c > 0) {
		print "static Rune "name"4[][2] = {" > file;
		for(j = 0; j < rune4c; j++) {
			print "\t{ 0x"rune4v0[j]", 0x"rune4v1[j]" }," > file;
		}
		print "};\n" > file;
	}

	# generate list of ranges
	if(rune2c > 0) {
		if(length(casev) > 0) {
			print "static Rune "name"2[][3] = {" > file;
			for(j = 0; j < rune2c; j++) {
				print "\t{ 0x"rune2v0[j]", 0x"rune2v1[j]", 0x"case2v[j]" }," > file;
			}
		} else {
			print "static Rune "name"2[][2] = {" > file
			for(j = 0; j < rune2c; j++) {
				print "\t{ 0x"rune2v0[j]", 0x"rune2v1[j]" }," > file;
			}
		}
		print "};\n" > file;
	}

	# generate list of singletons
	if(rune1c > 0) {
		if(length(casev) > 0) {
			print "static Rune "name"1[][2] = {" > file;
			for(j = 0; j < rune1c; j++) {
				print "\t{ 0x"rune1v[j]", 0x"case1v[j]" }," > file;
			}
		} else {
			print "static Rune "name"1[] = {" > file;
			for(j = 0; j < rune1c; j++) {
				print "\t0x"rune1v[j]"," > file;
			}
		}
		print "};\n" > file;
	}
	# generate lookup function
	print "int\nis"name"rune(Rune r)\n{" > file;
	if(rune4c > 0 || rune3c > 0)
		print "\tRune *match;\n" > file;
	if(rune4c > 0) {
		print "\tif((match = bsearch(&r, "name"4, nelem("name"4), sizeof *"name"4, &rune2cmp)))" > file;
		print "\t\treturn !((r - match[0]) % 2);" > file;
	}
	if(rune3c > 0) {
		print "\tif((match = bsearch(&r, "name"3, nelem("name"3), sizeof *"name"3, &rune2cmp)))" > file;
		print "\t\treturn !((r - match[0]) % 2);" > file;
	}
	if(rune2c > 0) {
		print "\tif(bsearch(&r, "name"2, nelem("name"2), sizeof *"name"2, &rune2cmp))\n\t\treturn 1;" > file;
	}
	if(rune1c > 0) {
		print "\tif(bsearch(&r, "name"1, nelem("name"1), sizeof *"name"1, &rune1cmp))\n\t\treturn 1;" > file;
	}
	print "\treturn 0;\n}" > file;

	# generate case conversion function
	if(length(casev) > 0) {
		print "\nint\nto"casename"rune(Rune r)\n{\n\tRune *match;\n" > file;
		if(rune4c > 0) {
			print "\tmatch = bsearch(&r, "name"4, nelem("name"4), sizeof *"name"4, &rune2cmp);" > file;
			print "\tif (match)" > file;
			print "\t\treturn ((r - match[0]) % 2) ? r : r - 1;" > file;
		}
		if(rune3c > 0) {
			print "\tmatch = bsearch(&r, "name"3, nelem("name"3), sizeof *"name"3, &rune2cmp);" > file;
			print "\tif (match)" > file;
			print "\t\treturn ((r - match[0]) % 2) ? r : r + 1;" > file;
		}
		if(rune2c > 0) {
			print "\tmatch = bsearch(&r, "name"2, nelem("name"2), sizeof *"name"2, &rune2cmp);" > file;
			print "\tif (match)" > file;
			print "\t\treturn match[2] + (r - match[0]);" > file;
		}
		if(rune1c > 0) {
			print "\tmatch = bsearch(&r, "name"1, nelem("name"1), sizeof *"name"1, &rune1cmp);" > file;
			print "\tif (match)" > file;
			print "\t\treturn match[1];" > file;
		}
		print "\treturn r;\n}" > file;
	}
}
