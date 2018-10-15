x=0.25:0.5:8;
A=1050*[1 0 1 0 ...
    0 1 1 1 ...
    0 1 0 1 ...
    1 0 0 1];

B=15*[1 1 1 1 ...
    0 1 .2 .2 ...
    1 1 1 1 ...
    0.2 1 0 1];

subplot(3,1,1)
stem(x,A,'.','LineWidth',15)
ylabel ({'SPD_1';'Counts (*10^3)'})
grid
title ('|0\rangle          |0\rangle           |1\rangle          |d\rangle           |1\rangle          |1\rangle           |0\rangle          |1\rangle')
subplot(3,1,2)
stem(x,B,'.','LineWidth',15)
ylabel ({'SPD_2';'Counts (*10^3)'})
grid

subplot(3,1,3)
for a=1:length(B)
if B(a)==15*.2
    B(a)=15*2;
end
end
stem(x,B,'.','LineWidth',15)
ylabel ({'Construtive';'Counts (*10^3)'})
grid
xlabel ({'Time (ns)'})
