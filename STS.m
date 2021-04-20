function encoded=STS(L,inf_transmitted)
m=0;
while (2^L)>=(((2^m-1)*(2^m-2))/6)
    m=m+1;
end

R=(hammgen(L))';

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
y=F1(1:2^L-1,1:2^m-1);
T=table(x,y);

for i=1:2^L-1
    if inf_transmitted==table2array(T(i,1))
        encoded=table2array(T(i,2));
    end
end
end 

