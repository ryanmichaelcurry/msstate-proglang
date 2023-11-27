PROGRAM FIB;
{ TIPS program for computing nth fibonacci number. }
VAR
    A: INTEGER;
    B: INTEGER;
    C: INTEGER;
    I: INTEGER;
    N: INTEGER;
BEGIN
    A := 0;
    B := 1;
    I := 1;
    WRITE('th Fibonacci Number: ');
    READ(N);
    IF N = 0 THEN
    BEGIN
        WRITE('');
        WRITE('You could have done that in your head...');
        WRITE('Answer: ');
        WRITE(A)
    END
    ELSE
    BEGIN
        WHILE I < N
        BEGIN
            C := A + B;
            A := B;
            B := C;
            I := I + 1
        END;
        WRITE('');
        WRITE('Answer: ');
        WRITE(B)
    END
END