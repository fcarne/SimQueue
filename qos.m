%% Theoritical results analysis
% DO NOT MOVE
load analysis\q_matrix.dat;
load analysis\node_info.dat;

Q = spconvert(q_matrix);
traffic = sparse(node_info(:,1));
mu = node_info(:,2);
lambda = full(Q\traffic);

gamma = full(sum(traffic));

T = 1 / gamma * sum(lambda ./ (mu - lambda ));

file = input('Select the output file (press Enter if absent): ');

if isempty(file) 
    fid = 1;
else
    fid = fopen(file, 'a+');
end

fmt=['Computed incoming traffic: [' repmat(' %.3f', 1, numel(lambda)) ' ]\n'];
fprintf(fid, fmt, lambda);

fmt=['Computed incoming T_i:     [' repmat(' %.3f', 1, numel(lambda)) ' ]\n'];
fprintf(fid, fmt, 1 ./ (mu - lambda));

fprintf(fid, "Computed QoS:              %.6f\n", T);

if fid ~= 1
    fclose(fid);
end
