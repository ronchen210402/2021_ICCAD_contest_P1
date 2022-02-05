
module top(clk, a, b, op, oe, y, parity, overflow, greater, is_eq,
     less);
  input clk, oe;
  input [7:0] a, b;
  input [1:0] op;
  output [7:0] y;
  output parity, overflow, greater, is_eq, less;
  wire clk, oe;
  wire [7:0] a, b;
  wire [1:0] op;
  wire [7:0] y;
  wire parity, overflow, greater, is_eq, less;
  wire n_41, n_47, n_56, n_64, n_69, n_140, n_143, n_144;
  wire n_185, n_190, n_191, n_192, n_193, n_194, n_195, n_264;
  wire n_339, n_360, n_369, n_410, n_413, n_445, n_446, n_450;
  wire n_451, n_453, n_455, n_464, n_465, n_467, n_468, n_474;
  wire n_475, n_476, n_483, n_484, n_486, n_496, n_497, n_502;
  wire n_503, n_505, n_519, n_520, n_521, n_522, n_524, n_535;
  wire n_536, n_537, n_538, n_539, n_540, n_541, n_543, n_544;
  wire n_545, n_546, n_547, n_548, n_549, n_550, n_551, n_552;
  wire n_553, n_554, n_555, n_561, n_568, n_569, n_570, n_575;
  wire n_576, n_579, n_584, n_585, n_588, n_595, n_596, n_597;
  wire n_602, n_603, n_606, n_611, n_612, n_615, n_620, n_621;
  wire n_626, n_627, n_634, n_635, n_636, n_641, n_642, n_651;
  wire n_652, n_653, n_654, n_657, n_662, n_663, n_670, n_671;
  wire n_672, n_677, n_678, n_683, n_684, n_687, n_694, n_695;
  wire n_696, n_701, n_702, n_705, n_710, n_711, n_714, n_725;
  wire n_726, n_727, n_728, n_729, n_732, n_735, n_744, n_745;
  wire n_746, n_747, n_756, n_757, n_758, n_759, n_768, n_769;
  wire n_770, n_771, n_780, n_781, n_782, n_783, n_802, n_803;
  wire n_804, n_805, n_806, n_807, n_811, n_812, n_819, n_820;
  assign overflow = y[7];
  assign y[6] = y[7];
  or g380 (n_264, wc, op[1]);
  not gc (wc, op[0]);
  or g1006 (n_446, wc0, a[1]);
  not gc0 (wc0, b[1]);
  or g1358 (n_464, wc1, a[3]);
  not gc1 (wc1, b[3]);
  or g1362 (n_468, wc2, a[4]);
  not gc2 (wc2, b[4]);
  or g1363 (n_475, wc3, a[5]);
  not gc3 (wc3, b[5]);
  or g1365 (n_465, b[7], wc4);
  not gc4 (wc4, a[7]);
  or g1366 (n_467, wc5, a[6]);
  not gc5 (wc5, b[6]);
  or g1367 (n_476, b[6], wc6);
  not gc6 (wc6, a[6]);
  nand g1802 (n_497, b[0], a[0]);
  or g1887 (n_484, wc7, a[2]);
  not gc7 (wc7, b[2]);
  or g1889 (n_496, wc8, a[0]);
  not gc8 (wc8, b[0]);
  or g1890 (n_483, wc9, a[7]);
  not gc9 (wc9, b[7]);
  or g1985 (n_535, a[4], b[4]);
  or g1993 (n_539, a[2], b[2]);
  or g1995 (n_540, a[3], b[3]);
  or g1997 (n_541, a[1], b[1]);
  or g2173 (n_802, wc10, n_195);
  not gc10 (wc10, n_451);
  or g2174 (n_803, n_451, wc11);
  not gc11 (wc11, n_195);
  nand g2175 (parity, n_802, n_803);
  or g2176 (n_804, wc12, n_450);
  not gc12 (wc12, n_502);
  or g2177 (n_805, n_502, wc13);
  not gc13 (wc13, n_450);
  nand g2178 (n_451, n_804, n_805);
  or g2179 (n_806, wc14, n_193);
  not gc14 (wc14, n_190);
  or g2180 (n_807, n_190, wc15);
  not gc15 (wc15, n_193);
  nand g2181 (n_502, n_806, n_807);
  nand g2184 (less, n_483, n_732);
  or g2185 (n_190, wc16, n_729);
  not gc16 (wc16, n_728);
  nand g2186 (n_732, n_536, n_465);
  nand g2189 (n_536, n_467, n_714);
  nand g2190 (n_728, n_140, n_544);
  or g2193 (n_811, wc17, n_191);
  not gc17 (wc17, n_503);
  or g2194 (n_812, n_503, wc18);
  not gc18 (wc18, n_191);
  nand g2195 (n_450, n_811, n_812);
  nand g2200 (n_729, n_726, n_727);
  or g2205 (n_69, n_505, n_264);
  not g2206 (y[7], n_69);
  or g2207 (n_191, wc19, n_783);
  not gc19 (wc19, n_782);
  or g2208 (n_714, wc20, n_505);
  not gc20 (wc20, n_476);
  or g2209 (n_726, wc21, n_140);
  not gc21 (wc21, n_545);
  or g2210 (n_711, n_710, wc22);
  not gc22 (wc22, n_483);
  or g2211 (n_544, n_696, wc23);
  not gc23 (wc23, n_486);
  nand g2212 (n_783, n_780, n_781);
  or g2213 (n_710, n_339, wc24);
  not gc24 (wc24, n_467);
  nand g2216 (n_545, n_701, n_702);
  nand g2217 (n_782, n_445, n_550);
  nand g2218 (n_505, n_455, n_705);
  or g2219 (n_819, wc25, n_194);
  not gc25 (wc25, n_192);
  or g2220 (n_820, n_192, wc26);
  not gc26 (wc26, n_194);
  nand g2221 (n_503, n_819, n_820);
  nand g2222 (n_696, n_694, n_695);
  nand g2223 (n_705, n_475, n_413);
  or g2224 (n_339, n_654, wc27);
  not gc27 (wc27, n_519);
  or g2225 (n_780, wc28, n_445);
  not gc28 (wc28, n_551);
  or g2226 (n_694, n_185, n_453);
  or g2227 (n_192, wc29, n_771);
  not gc29 (wc29, n_770);
  or g2228 (n_695, n_264, wc30);
  not gc30 (wc30, n_413);
  or g2229 (n_701, n_185, wc31);
  not gc31 (wc31, n_453);
  or g2230 (n_702, n_264, n_413);
  or g2231 (n_550, n_672, wc32);
  not gc32 (wc32, n_486);
  nand g2232 (n_672, n_670, n_671);
  nand g2233 (n_413, n_522, n_687);
  or g2234 (n_654, n_653, wc33);
  not gc33 (wc33, n_522);
  nand g2235 (n_770, n_64, n_546);
  nand g2236 (n_453, n_683, n_684);
  nand g2237 (n_771, n_768, n_769);
  nand g2238 (n_551, n_677, n_678);
  or g2239 (n_671, n_185, n_47);
  or g2240 (n_193, wc34, n_759);
  not gc34 (wc34, n_758);
  or g2241 (n_546, n_636, wc35);
  not gc35 (wc35, n_486);
  or g2242 (n_768, wc36, n_64);
  not gc36 (wc36, n_547);
  or g2243 (n_670, n_264, wc37);
  not gc37 (wc37, n_369);
  nand g2244 (n_683, n_535, n_47);
  nand g2245 (n_687, n_468, n_369);
  or g2246 (n_678, n_264, n_369);
  or g2247 (n_677, n_185, wc38);
  not gc38 (wc38, n_47);
  or g2248 (n_653, n_652, wc39);
  not gc39 (wc39, n_465);
  nand g2249 (n_547, n_641, n_642);
  nand g2250 (n_759, n_756, n_757);
  nand g2251 (n_636, n_634, n_635);
  nand g2252 (n_758, n_143, n_552);
  or g2253 (n_652, wc40, n_524);
  not gc40 (wc40, n_651);
  nand g2254 (n_369, n_519, n_657);
  nand g2255 (n_47, n_662, n_663);
  or g2256 (n_552, n_597, wc41);
  not gc41 (wc41, n_486);
  or g2257 (n_756, wc42, n_143);
  not gc42 (wc42, n_553);
  or g2258 (n_634, n_185, n_56);
  nand g2259 (n_657, n_464, n_410);
  or g2260 (n_194, wc43, n_747);
  not gc43 (wc43, n_746);
  nand g2261 (n_538, n_465, n_627);
  or g2262 (n_641, n_185, wc44);
  not gc44 (wc44, n_56);
  or g2263 (n_642, n_264, n_410);
  nand g2264 (n_662, n_540, n_56);
  nand g2265 (n_651, n_555, n_464);
  or g2266 (n_635, n_264, wc45);
  not gc45 (wc45, n_410);
  nand g2267 (n_56, n_620, n_621);
  nand g2268 (n_553, n_602, n_603);
  nand g2269 (n_747, n_744, n_745);
  or g2270 (n_627, n_626, wc46);
  not gc46 (wc46, n_483);
  nand g2271 (n_597, n_595, n_596);
  nand g2272 (n_410, n_521, n_615);
  nand g2273 (n_555, n_521, n_606);
  nand g2274 (n_746, n_144, n_548);
  or g2275 (n_548, n_570, wc47);
  not gc47 (wc47, n_486);
  or g2276 (n_595, n_185, n_41);
  nand g2277 (n_195, n_611, n_612);
  nand g2278 (n_626, n_537, n_467);
  nand g2279 (n_620, n_539, n_41);
  or g2280 (n_596, n_264, wc48);
  not gc48 (wc48, n_360);
  nand g2281 (n_615, n_484, n_360);
  or g2282 (n_603, n_264, n_360);
  or g2283 (n_602, n_185, wc49);
  not gc49 (wc49, n_41);
  or g2284 (n_744, wc50, n_144);
  not gc50 (wc50, n_549);
  nand g2285 (n_606, n_554, n_484);
  nand g2286 (n_611, n_543, n_474);
  nand g2287 (n_360, n_520, n_579);
  or g2288 (n_537, n_524, wc51);
  not gc51 (wc51, n_588);
  nand g2289 (n_554, n_520, n_735);
  nand g2290 (n_41, n_584, n_585);
  nand g2291 (n_549, n_575, n_576);
  nand g2292 (n_570, n_568, n_569);
  or g2293 (n_757, n_621, n_474);
  nand g2294 (n_143, n_521, n_484);
  nand g2295 (n_64, n_464, n_519);
  or g2296 (n_612, n_474, n_497);
  or g2297 (n_727, n_725, n_474);
  nand g2298 (n_543, n_496, n_561);
  or g2299 (n_769, n_663, n_474);
  nand g2300 (n_579, n_446, n_496);
  nand g2301 (n_524, n_455, n_476);
  nand g2302 (n_588, n_468, n_475);
  or g2303 (n_584, wc52, n_497);
  not gc52 (wc52, n_541);
  or g2304 (n_781, n_684, n_474);
  nand g2305 (n_144, n_446, n_520);
  or g2306 (n_568, n_185, wc53);
  not gc53 (wc53, n_497);
  or g2307 (n_569, n_264, wc54);
  not gc54 (wc54, n_496);
  or g2308 (n_735, n_561, wc55);
  not gc55 (wc55, n_446);
  or g2309 (n_575, n_185, n_497);
  or g2310 (n_576, n_264, n_496);
  or g2311 (n_745, n_585, n_474);
  nand g2312 (n_445, n_468, n_522);
  nand g2313 (n_140, n_455, n_475);
  nand g2314 (n_486, op[0], op[1]);
  or g2315 (n_520, wc56, b[1]);
  not gc56 (wc56, a[1]);
  or g2316 (n_521, wc57, b[2]);
  not gc57 (wc57, a[2]);
  or g2317 (n_519, wc58, b[3]);
  not gc58 (wc58, a[3]);
  or g2318 (n_522, wc59, b[4]);
  not gc59 (wc59, a[4]);
  nand g2319 (n_725, a[5], b[5]);
  nand g2320 (n_684, a[4], b[4]);
  nand g2321 (n_663, a[3], b[3]);
  or g2322 (n_474, wc60, op[0]);
  not gc60 (wc60, op[1]);
  nand g2323 (n_621, a[2], b[2]);
  nand g2324 (n_585, a[1], b[1]);
  or g2325 (n_185, op[1], op[0]);
  or g2326 (n_561, wc61, b[0]);
  not gc61 (wc61, a[0]);
  or g2327 (n_455, wc62, b[5]);
  not gc62 (wc62, a[5]);
  and g2328 (greater, n_538, n_339);
  and g2329 (y[1], n_194, wc63);
  not gc63 (wc63, n_69);
  and g2330 (is_eq, n_505, wc64);
  not gc64 (wc64, n_711);
  and g2331 (y[4], n_191, wc65);
  not gc65 (wc65, n_69);
  and g2332 (y[0], n_195, wc66);
  not gc66 (wc66, n_69);
  and g2333 (y[3], n_192, wc67);
  not gc67 (wc67, n_69);
  and g2334 (y[2], n_193, wc68);
  not gc68 (wc68, n_69);
  and g2335 (y[5], wc69, n_190);
  not gc69 (wc69, n_69);
endmodule
