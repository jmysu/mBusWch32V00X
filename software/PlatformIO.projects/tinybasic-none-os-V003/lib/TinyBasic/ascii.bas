10 REM ASCII.BAS
13 K=TIME()
15 F=50
20 FOR Y=-12 TO 12
30 FOR X=-39 TO 39
40 C=X*229/100
50 D=Y*416/100
60 A=C; B=D
70 I=0
90 Q=B/F; S=B-Q*F
100 T=(A*A-B*B)/F+C
110 B=2*(A*Q+A*S/F)+D
120 A=T
130 P=A/F; Q=B/F
140 IF (P*P+Q*Q)>4 GOTO 200
150 I=I+1;IF I<=15 GOTO 90
160 PRINT " ",
170 GOTO 300
200 IF I<10 PRINT I,; GOTO 300
210 IF I=10 PRINT "A",; GOTO 300
220 IF I=11 PRINT "B",; GOTO 300
230 IF I=12 PRINT "C",; GOTO 300
240 IF I=13 PRINT "D",; GOTO 300
250 IF I=14 PRINT "E",; GOTO 300
260 IF I=15 PRINT "F",
300 NEXT X
310 PRINT
320 NEXT Y
330 PRINT "TIME:",;PRINT TIME()-K,;PRINT "ms"
