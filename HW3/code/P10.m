    data_vec = zeros(8192, 13);
    y = zeros(1, 8192);
    E_in = zeros(1126, 1);
    E_out = zeros(1126, 1);
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
    
    
    count = 1;
    while count <= 1126
        in = 0;
        out = 0;
        indicies = zeros(1, 32);
        y_vec = zeros(32, 1);
        mat = zeros(32, 13);
        for i = 1:32
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
        in = in / 32;
        out = out / 8160;
        E_out(count, 1) = out;
        E_in(count, 1) = in;
        disp(count);
        count = count + 1;
    end
    disp(E_in);
    disp(E_out);
    disp(mean(E_out));
    scatter(E_in, E_out);
    xlabel({"E_{in}"});
    ylabel({"E_{out}"})
