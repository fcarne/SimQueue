%% Theoritical results analysis - DO NOT MOVE
% Computes T* using the QoS formula (assuming M|M|1 queues)

%% Load data
clear
clc

load analysis/q_matrix.dat;
load analysis/node_info.dat;

Q = spconvert(q_matrix);
traffic = sparse(node_info(:,1));
mu = node_info(:,2);

%% Compute values
lambda = full(Q\traffic); % solve the linear system

gamma = full(sum(traffic));

T = 1 / gamma * sum(lambda ./ (mu - lambda ));


%% Print results

% Ask whether to print results in a file or in the console
file = input('Select the output file (press Enter if absent): ');

if isempty(file) 
    fid = 1;
else
    fid = fopen(file, 'a+');
end


fprintf(fid, 'Total incoming traffic:    %.6f pck/s\n', gamma);

fmt=['Computed incoming traffic: [' repmat(' %.6f', 1, numel(lambda)) ' ]\n'];
fprintf(fid, fmt, lambda);

fmt=['Computed incoming T_i:     [' repmat(' %.6f', 1, numel(lambda)) ' ]\n'];
fprintf(fid, fmt, 1 ./ (mu - lambda));

fprintf(fid, 'Computed QoS:              %.6f s/pck\n', T);

if fid ~= 1
    fclose(fid);
end
