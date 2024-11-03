data_vec = zeros(8192, 13);
y = zeros(1, 8192);
E_in = zeros(80, 1);
E_out = zeros(80, 1);
file = fopen("./data.txt");

line = fgetl(file);
line_count = 0;
while ischar(line)
    line_string = string(line);
    parts = strsplit(line_string, ' ');
    %disp(parts);
    y(line_count + 1) = str2double(parts(1));
    x = zeros(13, 1);
    x(1) = 1;
    %disp(length(parts));
    for j = 2:13
        foo = strsplit(parts(j), ':');
        x(j) = str2double(foo(2));
    end
    data_vec(line_count + 1, :) = x;
    line_count = line_count + 1;
    disp(line_count);
    line = fgetl(file);
end
%disp(data_vec(8192, :));



for N = 25:25:2000
    count = 1;
    iin = 0;
    oout = 0;
    while count <= 16
        in = 0;
        out = 0;
        indicies = zeros(1, N);
        y_vec = zeros(N, 1);
        mat = zeros(N, 13);
        for i = 1:N
            r = randi([1, 8192]);
            indicies(1, i) = r;
            mat(i, :) = data_vec(r, :);
            y_vec(i, 1) = y(1, r);
        end
        w = pinv(mat) * y_vec;
        %disp(w);
        for i = 1:8192
            err = (dot(w, data_vec(i, :)) - y(1, i)) ^ 2;
            if ismember(i, indicies(1, :))
                in = in + err;
            else
                out = out + err;
            end
        end
        in = in / N;
        out = out / (8192 - N);
        iin = iin + in;
        oout = oout + out;
        count = count + 1;
    end
    oout = oout / 16;
    iin = iin / 16;
    E_out(N/25, 1) = oout;
    E_in(N/25, 1) = iin;

    disp(N);
end
vec = 25:25:2000;
plot(vec, E_in);
hold on;
plot(vec, E_out);
legend('E_{in}', 'E_{out}');
xlabel("N");
hold off;
disp(mean(E_out));
