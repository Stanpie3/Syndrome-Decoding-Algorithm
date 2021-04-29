function T=tableSTS(L)
m=0; %parameter for STS

while (2^L)>=(((2^m-1)*(2^m-2))/6)
    m=m+1;
end

R=(hammgen(L)); %Proof matrix of Hamming code
R=[R zeros(L,1)];
R=R';

d=(2^m-1)*(2^m-2)/6;
r=2^m-1;
Hb=hammgen(m);
H=gf(Hb);
Gb=gen2par(Hb);
F=zeros(d,(2^m-1));

G=gf(Gb);

g=1;
for i=1:r
    for j=i+1:r
        fg=H(:,i)+H(:,j);
        for k=1:r
            if fg==H(:,k) 
                F(g,i)=1;
                F(g,j)=1;
                F(g,k)=1;
                g=g+1;
            end
        end
    end
end

[~, idx] = unique(F,'rows');
F1 = F(sort(idx),:);

x=R;
y=F1(1:2^L,1:2^m-1);
T=table(x,y);
end