# See LICENSE file for copyright and license details.

BEGIN {
	FS = ";"
	# setup hexadecimal lookup table
	for(i = 0; i < 16; i++)
		hex[sprintf("%X",i)] = i;
}

$3  ~ /^L/ { alphav[alphac++] = $1; }
$3  ~ /^Z/ { spacev[spacec++] = $1; }
$3 == "Lu" { upperv[upperc++] = $1; }
$3 == "Ll" { lowerv[lowerc++] = $1; }
$3 == "Lt" { titlev[titlec++] = $1; }
$3 == "Nd" { digitv[digitc++] = $1; }

END {
	print "/* Automatically generated from "FILENAME" by mkrunetype.awk */\n"

	mkis("alpha", alphav, alphac);
	mkis("space", spacev, spacec);
	mkis("upper", upperv, upperc);
	mkis("lower", lowerv, lowerc);
	mkis("title", titlev, titlec);
	mkis("digit", digitv, digitc);
}

function code(s) {
	x = 0;
	for(i = 1; i <= length(s); i++) {
		c = substr(s, i, 1);
		x = (x*16) + hex[c];
	}
	return x;
}

function mkis(name, runev, runec) {
	rune1c = 0;
	rune2c = 0;

	for(j = k = 0; j < runec; j++) {
		if(j+1 == runec || code(runev[j+1]) != code(runev[j])+1) {
			if(j == k) {
				rune1v[rune1c] = runev[j];
				rune1c++;
			}
			else {
				rune2v0[rune2c] = runev[k];
				rune2v1[rune2c] = runev[j];
				rune2c++;
			}
			k = j+1;
		}
	}
	if(rune2c > 0) {
		print "static Rune "name"2[][2] = {";
		for(j = 0; j < rune2c; j++) {
			print "\t{ 0x"rune2v0[j]", 0x"rune2v1[j]" },";
		}
		print "};\n";
	}
	if(rune1c > 0) {
		print "static Rune "name"1[] = {";
		for(j = 0; j < rune1c; j++) {
			print "\t0x"rune1v[j]",";
		}
		print "};\n";
	}
	print "int\nis"name"rune(Rune r)\n{";
	if(rune2c > 0)
		print "\tif(bsearch(&r, "name"2, nelem("name"2), sizeof *"name"2, &rune2cmp))\n\t\treturn 1;";
	if(rune1c > 0)
		print "\tif(bsearch(&r, "name"1, nelem("name"1), sizeof *"name"1, &rune1cmp))\n\t\treturn 1;";
	print "\treturn 0;\n}\n";
}
