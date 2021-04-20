clc;
clear all;
L=32;
M = 2^L; %// alphabet size\

F = 4096;
s = floor(F/L);
u = 500:-10:2;

%%%%%%%%%%%%%%%%%%%%%%%%%2 errors case ??????????? ??? t=2%%%%%%%%%%%%%%%%%
N=31;
R=31-6;
K=N-R;
Z=bchgenpoly(N,K);
H = cyclgen(N,double(Z.x),'system');
G=gen2par(H);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fe=30;
prob = 1 - ((s-1)/s).^u;
Itr=zeros(1,length(prob));
Pkt_Err=zeros(1,length(prob));
Err=zeros(1,length(prob));
Ranks=zeros(1,length(prob));
Errors=zeros(1,length(prob));
SER=1-(1-prob).^L;
BER=1-(1-prob).^(1/L);

 for t=1:length(prob)
     pr_inp=prob(t);
     Itr(t)=0;
     Pkt_Err(t)=0;
     Err(t)=0;
     while Pkt_Err(t)<fe
         inf_transmitted=randi(2,L,K)-1;
         Itr(t)=Itr(t)+1;
         encoded=mod(inf_transmitted*G,2);
         errors = zeros(L, N);
         cols = double(rand(1,N)<=pr_inp);
         if any(cols)
            errors(:, cols > 0) = rand(L, sum(cols)) >= 0.5;
         end
         received = double((encoded + errors) > 0 );
         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
         received_q = bi2de(received');
         [decoded,r]=decode_ML(received_q,H,L);
         Ranks(t)=Ranks(t)+r;
         decoded2=de2bi(decoded,L)';
         if any(decoded2-encoded)
             Pkt_Err(t)=Pkt_Err(t)+1;
             disp(sprintf('Pkt_Err=%d and Itr=%d FER=%f_probIn=%f_users=%d',Pkt_Err(t),Itr(t),Pkt_Err(t)/Itr(t), pr_inp, u(t)));
             save(sprintf('result_BER=%f_SER=%f_BCH_N=%d_k=%d_L=%d.mat', BER(t), prob(t), N, K, L));
         end;
     end;
     save(sprintf('result_BER=%f_SER=%f_BCH_N=%d_k=%d_L=%d_users=%d.mat', BER(t), prob(t), N, K, L, u(t)));
 end;
 
figure;
plot(prob, Pkt_Err./Itr, '-r');
set(gca, 'YScale', 'log');        
         
         