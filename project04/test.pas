PROGRAM TEST;
{ TIPS program for computing some areas. }
VAR
    PI: REAL;
    RADIUS: REAL;
    AREA: REAL;
BEGIN
    PI := 3.14159;
    WRITE('Enter Radius: ');
    READ(RADIUS);
    AREA := RADIUS * 2 * PI;
    WRITE('The area of the circle: ');
    WRITE(AREA)
END