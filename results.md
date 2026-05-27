# Firmware run with CAN simulation

west build -b native_sim apps/zephyr-can-monitor -d build/native_sim --pristine && \
timeout 5 ./build/native_sim/zephyr/zephyr.exe 2>&1 | tee app.log

CAN logger started  Logging immediately
[0] ID:0x101 DLC:8 Data:3B DB 31 CC B6 D5 08 AA
[0] ID:0x102 DLC:8 Data:E1 BE 87 E4 7B 88 AE B1
[0] ID:0x103 DLC:8 Data:E6 88 9E FB 2D 97 50 16
[110] ID:0x101 DLC:8 Data:28 31 2F C0 E6 44 E9 BE
[220] ID:0x101 DLC:8 Data:36 DC AF EF 48 D1 ED 48
[330] ID:0x101 DLC:8 Data:6D AA 0F F2 91 F7 B1 22
[440] ID:0x101 DLC:8 Data:DF 53 C4 8E 61 08 F9 66
[510] ID:0x102 DLC:8 Data:94 CB E2 6D F7 0B B9 67
[550] ID:0x101 DLC:8 Data:E6 CC 6F 5D A0 95 55 5C
[660] ID:0x101 DLC:8 Data:4D 02 67 9F 89 A0 52 11
[770] ID:0x101 DLC:8 Data:56 D8 B3 DA 64 D2 1E C8
[880] ID:0x101 DLC:8 Data:5E 82 86 BC 23 5A 02 44
[990] ID:0x101 DLC:8 Data:0C 34 24 A2 AC F8 FD 71
[1010] ID:0x103 DLC:8 Data:5C 4A 1B CA DD 27 BD 58
[1020] ID:0x102 DLC:8 Data:45 09 7B 00 BA D7 AC A1
[1100] ID:0x101 DLC:8 Data:32 52 7E 6E 64 E7 9F 33
[1210] ID:0x101 DLC:8 Data:E3 74 39 4D B8 AB 2D 95
[1320] ID:0x101 DLC:8 Data:DD DB DF E2 83 8E 9A 8C
[1430] ID:0x101 DLC:8 Data:96 43 53 C8 50 FE F7 2C
[1530] ID:0x102 DLC:8 Data:EB 9F 19 15 53 65 96 B6
[1540] ID:0x101 DLC:8 Data:3D B1 C8 6B 2B 19 70 1B
[1650] ID:0x101 DLC:8 Data:5C 79 4D C9 AD 9F 2C 3F
[1760] ID:0x101 DLC:8 Data:5E BD 5A D5 E1 C3 CF EB
[1870] ID:0x101 DLC:8 Data:5C 2F FF 31 2C DF 99 C9
[1980] ID:0x101 DLC:8 Data:19 88 E8 77 35 5A 9A 83
[2020] ID:0x103 DLC:8 Data:C6 29 A3 A9 87 D8 08 E8
[2040] ID:0x102 DLC:8 Data:65 5D 55 8D FC 11 55 A0
[2090] ID:0x101 DLC:8 Data:92 F2 0F 6A D8 F1 37 24
[2200] ID:0x101 DLC:8 Data:2C 7F D6 00 1C 45 FF 74
[2310] ID:0x101 DLC:8 Data:23 AC 32 EB BB 5E 14 C8
[2420] ID:0x101 DLC:8 Data:B9 62 2F 29 6D 27 7F F3
[2530] ID:0x101 DLC:8 Data:29 7E FE 85 E6 74 48 A1
[2550] ID:0x102 DLC:8 Data:56 BD 27 E3 A3 CF 1D 83
[2640] ID:0x101 DLC:8 Data:25 A9 49 3F D3 CE 4F AF
[2750] ID:0x101 DLC:8 Data:BE 72 93 59 5B B7 C6 E4
[2860] ID:0x101 DLC:8 Data:58 F6 B4 1F 85 5A 6F 6C
[2970] ID:0x101 DLC:8 Data:60 DE BD 84 E2 AF FC 68
[3030] ID:0x103 DLC:8 Data:B6 7E 72 16 D5 BF 66 41
[3060] ID:0x102 DLC:8 Data:ED A2 B5 42 54 96 66 5D
[3080] ID:0x101 DLC:8 Data:ED 17 F7 65 00 C9 52 19
[3190] ID:0x101 DLC:8 Data:01 E0 9D 1E 63 F8 7B 86
[3300] ID:0x101 DLC:8 Data:81 D3 5D 47 E1 F6 00 6D
[3410] ID:0x101 DLC:8 Data:C9 74 BD 9D 9E 1E E3 10
[3520] ID:0x101 DLC:8 Data:E7 F7 E0 C2 47 68 23 E4
[3570] ID:0x102 DLC:8 Data:36 A7 E3 81 BA FB 7C 3F
[3630] ID:0x101 DLC:8 Data:FC 8B 40 D9 B0 B0 9B C4
[3740] ID:0x101 DLC:8 Data:6B F8 75 3A 8F 6E 1F A7
[3850] ID:0x101 DLC:8 Data:68 61 BD F0 0D E1 35 7C
[3960] ID:0x101 DLC:8 Data:CE A2 F6 DD B0 96 77 D0
[4040] ID:0x103 DLC:8 Data:35 00 75 CA C8 8D DC D0
[4070] ID:0x101 DLC:8 Data:87 19 52 CC A8 2B FA 33
[4080] ID:0x102 DLC:8 Data:02 A0 56 4D 7C 89 41 15
[4180] ID:0x101 DLC:8 Data:02 0D D0 16 F1 46 DF B3
[4290] ID:0x101 DLC:8 Data:E5 1D 19 FD E8 27 A1 FB
[4400] ID:0x101 DLC:8 Data:08 02 31 33 90 0B 22 B6
[4510] ID:0x101 DLC:8 Data:84 41 00 2F 1C 4C FC 66
[4590] ID:0x102 DLC:8 Data:1F 06 2A 9B 14 C2 36 F6
[4620] ID:0x101 DLC:8 Data:3E AD 03 03 76 7F 26 04
[4730] ID:0x101 DLC:8 Data:73 29 9D D1 18 76 03 B5
[4840] ID:0x101 DLC:8 Data:EA F9 CC 06 19 EB CE 7D
[4950] ID:0x101 DLC:8 Data:C6 CA C5 8D 32 3D 9A 26

# Unit Test

west twister -T apps/zephyr-can-monitor/tests -p native_sim --inline-logs

Renaming previous output directory to /mnt/data/zephyr/twister-out.3
INFO    - Using Ninja..
INFO    - Zephyr version: v4.4.0-3651-ge07c8cf4d645
INFO    - Using 'zephyr/gnu' toolchain variant.
INFO    - Building initial testsuite list...
INFO    - Built testsuite list in 0.00 seconds
INFO    - Writing JSON report /mnt/data/zephyr/twister-out/testplan.json
INFO    - JOBS: 8
INFO    - Adding tasks to the queue...
INFO    - Added initial list of jobs to queue
INFO    - Total complete:    3/   3  100%  built (not run):    0, filtered:    0, failed:    0, error:    0
INFO    - 3 test scenarios (3 configurations) selected, 0 configurations filtered (0 by static filter, 0 at runtime).
INFO    - 3 of 3 executed test configurations passed (100.00%), 0 built (not run), 0 failed, 0 errored, with no warnings in 15.66 seconds.
INFO    - 24 of 24 executed test cases passed (100.00%) on 1 out of total 1535 platforms (0.07%).
INFO    - 3 test configurations executed on platforms, 0 test configurations were only built.
INFO    - Saving reports...
INFO    - Writing JSON report /mnt/data/zephyr/twister-out/twister.json
INFO    - Writing xunit report /mnt/data/zephyr/twister-out/twister.xml...
INFO    - Writing xunit report /mnt/data/zephyr/twister-out/twister_report.xml...
INFO    - Run completed

