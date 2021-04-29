clc;
clear all;
L=9; 
M=2^L; %alphabet size

m=0;
while (2^L)>=(((2^m-1)*(2^m-2))/6)
    m=m+1;
end

T=tableSTS(L);

F = 1152; %the number of subchannels
s = floor(F/L); %number of slots in channel
u = 500:-10:100; %the number of users for simulations
prob = 1 - ((s-1)/s).^u; %the probability of collision
Itr=zeros(1,length(prob)); %the number of iterations for particular number of users
Pkt_Err=zeros(1,length(prob)); %the number of destroying decoding algorithm
t=zeros(1,length(prob)); %the order of collision
w=zeros(1,length(prob)); 
fe=30;

%Probabilities of different multiplicity of collisions
    Pcolt=zeros(2,20);
    t=1;
    while t<=20
        Pcolt(1,t)=nchoosek(u-1,t)*((1/s).^t)*((1-1/s)^(u-t-1));
        t=t+1;
    end
    plot(Pcolt(1,:),Pcolt(2,:));


%BCH coder
N=31;
K=11;
Z=bchgenpoly(N,K);
H = cyclgen(N,double(Z.x),'system');
G=gen2par(H);
    
 for k=1:length(prob)
     pr_inp=prob(k);
     Itr(k)=0;
     Pkt_Err(k)=0;
     while (Pkt_Err(k)<fe)||(Itr(k)<60)
        
        Itr(k)=Itr(k)+1;
        %The number of collisions
        col=zeros(1,u(k)-1);
        for i=1:u(k)-1
            col(i)=randi(s);
        end

        t(k)=0;
        for j=1:u(k)-1
            if col(j)==1
                t(k)=t(k)+1;
            end
        end

        %Encoding (BCH, STS)
        inf_transmitted1=randi(2,L,K)-1;
        inf_transmitted=mod(inf_transmitted1*G,2);
        encoded=zeros((2^m-1),N);
         for i=1:N
            for j=1:2^L
                if inf_transmitted(:,i)==table2array(T(j,1))'
                    encoded(:,i)=table2array(T(j,2))';
                end
            end
         end

         %Adding errors
         received=encoded;
         errors = zeros((2^m-1),N);
         cols = double(rand(1,N)<=pr_inp);
         if any(cols)
            for i=1:N
                if cols(i)==1
                    for j=1:t(k)
                        received(:,i)=double((received(:,i) + table2array(T(randi(2^m-1),2))') > 0 );
                    end
                end
            end
         end

         w=[];
         for i=1:N
             w=[w sum(dec2bin(received(:,i)).'=='1')];
         end
         
         received_q = bi2de(received','left-msb');
         decoded=MEX-file(received_q,H,L);
         decoded2=de2bi(decoded,2^m-1)';
         if any(decoded2-encoded)
             Pkt_Err(t)=Pkt_Err(t)+1;
         end
     end
 end
 
plot(prob, Pkt_Err./Itr, '-r');
set(gca, 'YScale', 'log');

R=((U*(K/N))/S).*(1-Pkt_Err./Itr);
plot(U/S,R,'-b');
 
 
