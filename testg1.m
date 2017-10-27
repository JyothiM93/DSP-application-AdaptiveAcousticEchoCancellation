clc;
clear all;
close all;
[x, Fs] = audioread('audioclip.wav');    %Far-end signal
load impresp                             % Loading the impulse response
L=length(h);                             %length of impulse 
L = 64;
N=length(x);                             %length of input signal
w=zeros(L,1);                            %initializing to zeroes
xin=zeros(L,1); 

%Microphone signal
EchoSignal=filter(h,1,x);                %producing the echo signal
d=EchoSignal;                              

%Make column vectors
x=x(:); 
d=d(:); 
a = input('enter a value');              %initializing the values of a and c for step size
c = input('enter c value');

%initialization of variables
y = 0*x;
e = 0*x;
powerD = 0*x;
powerE = 0*x;
%NLMS algorithm
for i=1:N
xin = [x(i); xin(1:end-1)];
y(i)=w'*xin; 
error=d(i)-y(i);                        %ERROR
e(i)=error;                             %Store estimation error                       
mu=a/(c+xin'*xin);                      %Calculate Step-size
wtemp = w + 2*mu*error*xin;             %Update filter
w = wtemp;
powerD(i) = abs(d(i))^2;                %Power of Microphone signal
powerE(i)=abs(e(i))^2;                  %power of Error signal

end
for i=1:N-L
 %Echo Return Loss Enhancement
 ERLE(i)=10*log10(mean(powerD(i:i+L))/mean(powerE(i:i+L)));     %Calculating the ERLE
end
figure(1)

%-------Echo signal--------
subplot(4,1,1)
plot(EchoSignal)                                       %plotting the echo signal
xlabel('time (samples)','FontSize', 18);
ylabel('echo(n)','FontSize', 18);
title('ECHO SIGNAL: echo(n)','FontSize', 18)
grid on
axis([0 N -1 1]);

%-------Input signal-------------------
subplot(4,1,2)
plot(x)                                                %plotting the input signal
xlabel('time (samples)','FontSize', 18);
ylabel('x(n)','FontSize', 18);
title('INPUT SIGNAL: x(n)','FontSize', 18)
grid on
axis([0 N -1 1]);

%-------Output signal x(n)----------------
subplot(4,1,3)
plot(y)                                                %plotting output signal
xlabel('time (samples)','FontSize', 18);
ylabel('y(n)','FontSize', 18);
title('OUTPUT SIGNAL y(n)','FontSize', 18)
grid on
axis([0 N -1 1]);

%-------Error signal x(n)-----------------
subplot(4,1,4)
plot(e,'red')                                         %plotting the error signal
xlabel('time (samples)','FontSize', 18);
ylabel('E(n)','FontSize', 18);
title('ERROR SIGNAL: e(n)','FontSize', 18)
axis([0 N -1 1]);
grid on

figure(3)
plot(ERLE)
hold on;                                            %plotting the ERLE w.r.t desired signal
plot(d,'r');                        
xlabel('Sample number (n)','FontSize', 20);
ylabel('Desired signal/Error signal (dB)','FontSize', 20);
legend('ERLE', 'DESIRED SIGNAL')
title('ECHO RETURN LOSS ENHANCEMENT','FontSize', 20)
axis([0 N -20 40]);
grid on 