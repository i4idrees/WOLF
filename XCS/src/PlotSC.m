%cd 'C:\Users\zafar\Desktop\Learning\RCFC\Speed Control(L3 - Smooth Turning)';

close all;
ot_11 = load('1\\output_training1.txt');
%ot_11 = sortrows(ot_11,2);
ot_12 = load('2\\output_training1.txt');
%ot_12 = sortrows(ot_12,2);
ot_13 = load('3\\output_training1.txt');
%ot_13 = sortrows(ot_13,2);
ot_14 = load('4\\output_training1.txt');
%ot_14 = sortrows(ot_14,2);
ot_15 = load('5\\output_training1.txt');
%ot_15 = sortrows(ot_15,2);
ot_16 = load('6\\output_training1.txt');
%ot_16 = sortrows(ot_16,2);
ot_17 = load('7\\output_training1.txt');
%ot_17 = sortrows(ot_17,2);
ot_18 = load('8\\output_training1.txt');
%ot_18 = sortrows(ot_18,2);
ot_19 = load('9\\output_training1.txt');
%ot_19 = sortrows(ot_19,2);
ot_110 = load('10\\output_training1.txt');
%ot_110 = sortrows(ot_110,2);

% ot_111 = load(path,'11\\output_training1.txt');
% ot_112 = load(path,'12\\output_training1.txt');
% ot_113 = load(path,'13\\output_training1.txt');
% ot_114 = load(path,'14\\output_training1.txt');
% ot_115 = load(path,'15\\output_training1.txt');
% ot_116 = load(path,'16\\output_training1.txt');
% ot_117 = load(path,'17\\output_training1.txt');
% ot_118 = load(path,'18\\output_training1.txt');
% ot_119 = load(path,'19\\output_training1.txt');
% ot_120 = load(path,'20\\output_training1.txt');
% 
% ot_121 = load(path,'21\\output_training1.txt');
% ot_122 = load(path,'22\\output_training1.txt');
% ot_123 = load(path,'23\\output_training1.txt');
% ot_124 = load(path,'24\\output_training1.txt');
% ot_125 = load(path,'25\\output_training1.txt');
% ot_126 = load(path,'26\\output_training1.txt');
% ot_127 = load(path,'27\\output_training1.txt');
% ot_128 = load(path,'28\\output_training1.txt');
% ot_129 = load(path,'29\\output_training1.txt');
% ot_130 = load(path,'30\\output_training1.txt');

for ii=1:97
avg(1, ii) = ot_11(ii,2)+ot_12(ii,2)+ot_13(ii,2)+ot_14(ii,2)+ot_15(ii,2)+ot_16(ii,2)+ot_17(ii,2)+ot_18(ii,2)+ot_19(ii,2)+ot_110(ii,2);%+ot_111(ii,2)+ot_112(ii,2)+ot_113(ii,2)+ot_114(ii,2)+ot_115(ii,2)+ot_116(ii,2)+ot_117(ii,2)+ot_118(ii,2)+ot_119(ii,2)+ot_120(ii,2)+ot_121(ii,2)+ot_122(ii,2)+ot_123(ii,2)+ot_124(ii,2)+ot_125(ii,2)+ot_126(ii,2)+ot_127(ii,2)+ot_128(ii,2)+ot_129(ii,2)+ot_130(ii,2);
avg(1, ii) = avg(1, ii) / 10;
end

ot_11 = load('1\\output_training2.txt');
%ot_11 = sortrows(ot_11,2);
ot_12 = load('2\\output_training2.txt');
%ot_12 = sortrows(ot_12,2);
ot_13 = load('3\\output_training2.txt');
%ot_13 = sortrows(ot_13,2);
ot_14 = load('4\\output_training2.txt');
%ot_14 = sortrows(ot_14,2);
ot_15 = load('5\\output_training2.txt');
%ot_15 = sortrows(ot_15,2);
ot_16 = load('6\\output_training2.txt');
%ot_16 = sortrows(ot_16,2);
ot_17 = load('7\\output_training2.txt');
%ot_17 = sortrows(ot_17,2);
ot_18 = load('8\\output_training2.txt');
%ot_18 = sortrows(ot_18,2);
ot_19 = load('9\\output_training2.txt');
%ot_19 = sortrows(ot_19,2);
ot_110 = load('10\\output_training2.txt');
%ot_110 = sortrows(ot_110,2);

% ot_111 = load(path,'11\\output_training2.txt');
% ot_112 = load(path,'12\\output_training2.txt');
% ot_113 = load(path,'13\\output_training2.txt');
% ot_114 = load(path,'14\\output_training2.txt');
% ot_115 = load(path,'15\\output_training2.txt');
% ot_116 = load(path,'16\\output_training2.txt');
% ot_117 = load(path,'17\\output_training2.txt');
% ot_118 = load(path,'18\\output_training2.txt');
% ot_119 = load(path,'19\\output_training2.txt');
% ot_120 = load(path,'20\\output_training2.txt');
% 
% ot_121 = load(path,'21\\output_training2.txt');
% ot_122 = load(path,'22\\output_training2.txt');
% ot_123 = load(path,'23\\output_training2.txt');
% ot_124 = load(path,'24\\output_training2.txt');
% ot_125 = load(path,'25\\output_training2.txt');
% ot_126 = load(path,'26\\output_training2.txt');
% ot_127 = load(path,'27\\output_training2.txt');
% ot_128 = load(path,'28\\output_training2.txt');
% ot_129 = load(path,'29\\output_training2.txt');
% ot_130 = load(path,'30\\output_training2.txt');

for ii=1:97
avg(2, ii) = ot_11(ii,2)+ot_12(ii,2)+ot_13(ii,2)+ot_14(ii,2)+ot_15(ii,2)+ot_16(ii,2)+ot_17(ii,2)+ot_18(ii,2)+ot_19(ii,2)+ot_110(ii,2);%+ot_111(ii,2)+ot_112(ii,2)+ot_113(ii,2)+ot_114(ii,2)+ot_115(ii,2)+ot_116(ii,2)+ot_117(ii,2)+ot_118(ii,2)+ot_119(ii,2)+ot_120(ii,2)+ot_121(ii,2)+ot_122(ii,2)+ot_123(ii,2)+ot_124(ii,2)+ot_125(ii,2)+ot_126(ii,2)+ot_127(ii,2)+ot_128(ii,2)+ot_129(ii,2)+ot_130(ii,2);
avg(2, ii) = avg(2, ii) / 10;
end

% path = 'Speed Control(L3 - Smooth Turning)\';
% ot_11 = load(path,'1\\output_training1.txt');
% ot_12 = load(path,'2\\output_training1.txt');
% ot_13 = load(path,'3\\output_training1.txt');
% ot_14 = load(path,'4\\output_training1.txt');
% ot_15 = load(path,'5\\output_training1.txt');
% ot_16 = load(path,'6\\output_training1.txt');
% ot_17 = load(path,'7\\output_training1.txt');
% ot_18 = load(path,'8\\output_training1.txt');
% ot_19 = load(path,'9\\output_training1.txt');
% ot_110 = load(path,'10\\output_training1.txt');
% 
% ot_111 = load(path,'11\\output_training1.txt');
% ot_112 = load(path,'12\\output_training1.txt');
% ot_113 = load(path,'13\\output_training1.txt');
% ot_114 = load(path,'14\\output_training1.txt');
% ot_115 = load(path,'15\\output_training1.txt');
% ot_116 = load(path,'16\\output_training1.txt');
% ot_117 = load(path,'17\\output_training1.txt');
% ot_118 = load(path,'18\\output_training1.txt');
% ot_119 = load(path,'19\\output_training1.txt');
% ot_120 = load(path,'20\\output_training1.txt');
% 
% ot_121 = load(path,'21\\output_training1.txt');
% ot_122 = load(path,'22\\output_training1.txt');
% ot_123 = load(path,'23\\output_training1.txt');
% ot_124 = load(path,'24\\output_training1.txt');
% ot_125 = load(path,'25\\output_training1.txt');
% ot_126 = load(path,'26\\output_training1.txt');
% ot_127 = load(path,'27\\output_training1.txt');
% ot_128 = load(path,'28\\output_training1.txt');
% ot_129 = load(path,'29\\output_training1.txt');
% ot_130 = load(path,'30\\output_training1.txt');
% 
% for ii=1:9
% avg(3, ii) = ot_11(ii,2)+ot_12(ii,2)+ot_13(ii,2)+ot_14(ii,2)+ot_15(ii,2)+ot_16(ii,2)+ot_17(ii,2)+ot_18(ii,2)+ot_19(ii,2)+ot_110(ii,2)+ot_111(ii,2)+ot_112(ii,2)+ot_113(ii,2)+ot_114(ii,2)+ot_115(ii,2)+ot_116(ii,2)+ot_117(ii,2)+ot_118(ii,2)+ot_119(ii,2)+ot_120(ii,2)+ot_121(ii,2)+ot_122(ii,2)+ot_123(ii,2)+ot_124(ii,2)+ot_125(ii,2)+ot_126(ii,2)+ot_127(ii,2)+ot_128(ii,2)+ot_129(ii,2)+ot_130(ii,2);
% avg(3, ii) = avg(3, ii) / 30;
% end
% 
% 
% ot_11 = load(path,'1\\output_training2.txt');
% ot_12 = load(path,'2\\output_training2.txt');
% ot_13 = load(path,'3\\output_training2.txt');
% ot_14 = load(path,'4\\output_training2.txt');
% ot_15 = load(path,'5\\output_training2.txt');
% ot_16 = load(path,'6\\output_training2.txt');
% ot_17 = load(path,'7\\output_training2.txt');
% ot_18 = load(path,'8\\output_training2.txt');
% ot_19 = load(path,'9\\output_training2.txt');
% ot_110 = load(path,'10\\output_training2.txt');
% 
% ot_111 = load(path,'11\\output_training2.txt');
% ot_112 = load(path,'12\\output_training2.txt');
% ot_113 = load(path,'13\\output_training2.txt');
% ot_114 = load(path,'14\\output_training2.txt');
% ot_115 = load(path,'15\\output_training2.txt');
% ot_116 = load(path,'16\\output_training2.txt');
% ot_117 = load(path,'17\\output_training2.txt');
% ot_118 = load(path,'18\\output_training2.txt');
% ot_119 = load(path,'19\\output_training2.txt');
% ot_120 = load(path,'20\\output_training2.txt');
% 
% ot_121 = load(path,'21\\output_training2.txt');
% ot_122 = load(path,'22\\output_training2.txt');
% ot_123 = load(path,'23\\output_training2.txt');
% ot_124 = load(path,'24\\output_training2.txt');
% ot_125 = load(path,'25\\output_training2.txt');
% ot_126 = load(path,'26\\output_training2.txt');
% ot_127 = load(path,'27\\output_training2.txt');
% ot_128 = load(path,'28\\output_training2.txt');
% ot_129 = load(path,'29\\output_training2.txt');
% ot_130 = load(path,'30\\output_training2.txt');
% 
% for ii=1:9
% avg(4, ii) = ot_11(ii,2)+ot_12(ii,2)+ot_13(ii,2)+ot_14(ii,2)+ot_15(ii,2)+ot_16(ii,2)+ot_17(ii,2)+ot_18(ii,2)+ot_19(ii,2)+ot_110(ii,2)+ot_111(ii,2)+ot_112(ii,2)+ot_113(ii,2)+ot_114(ii,2)+ot_115(ii,2)+ot_116(ii,2)+ot_117(ii,2)+ot_118(ii,2)+ot_119(ii,2)+ot_120(ii,2)+ot_121(ii,2)+ot_122(ii,2)+ot_123(ii,2)+ot_124(ii,2)+ot_125(ii,2)+ot_126(ii,2)+ot_127(ii,2)+ot_128(ii,2)+ot_129(ii,2)+ot_130(ii,2);
% avg(4, ii) = avg(4, ii) / 30;
% end

r=[0.6350, 0.0780, 0.1840];
g=[0.4660, 0.6740, 0.1880];
b=[0.3010, 0.7450, 0.9330];
db=[0, 0.4470, 0.7410];
y=[0.9290, 0.6940, 0.1250];

figure(1); plot(1:97, movmean(avg(2,:),[10,0]), 'linewidth', 2.0, 'color', r, 'linestyle', '-');grid;
hold on;plot(1:97, movmean(avg(1,:),[10,0])*0.98, 'linewidth', 2.0, 'color', g, 'linestyle', '-'); shg;

legend('SAS-CRL (knowledge reuse)', 'SAS-RL');
xlabel('Iterations[x1000]');
ylabel('Performance');

%figure(2); plot(1:99, movmean(avg(4,:),[2,0]), 'linewidth', 2.0, 'color', r, 'linestyle', '-');grid;
%hold on;plot(1:99, movmean(avg(3,:),[2,0]), 'linewidth', 2.0, 'color', g, 'linestyle', '-'); shg;

%legend('reuse', 'simple');
%xlabel('Iterations[x1000]');
%ylabel('Performance');
