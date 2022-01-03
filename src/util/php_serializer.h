namespace App::Util::Php::Serializer {

    std::pair<int, nlohmann::json> unserialize(std::string_view raw_data) {
        nlohmann::json res;
        int state = 0; // 0 - type, 1 - length, 2 - data
        int type = 0; // 0 - null, 1 - array,
        int size = 0;
        int sign = 1;// size of string /array /object
        for (int i = 0; i < raw_data.size();i++) {
            switch(state) {
                case 0:
                    switch (raw_data[i]) {
                        case 'N': // null
                            return {2, res};
                            break;
                        case 'b':
                            return {4, raw_data[i+2] == '1'};
                            break;
                        case 'r':
                            size = i;
                            while (raw_data[i]!=';') i++;
                            return {i-size, res};
                            break;
                        case 'i':
                            sign = 1;
                            size = 0;
                            state = 10;
                            i++;
                            break;
                        case 's': //string s:{size}:"{content}";
                            size = 0;
                            state = 20;
                            i++;
                            break;
                        case 'a': // array a:{size}:"{content}";
                            size = 0;
                            state = 30;
                            res = {};
                            i++;
                            break;
                        case 'O': // object
                            size = 0;
                            state = 40;
                            res = {};
                            i++;
                            break;
                    }
                    break;
                case 10: // integer catch sign
                    if (raw_data[i] == '-') {
                        sign = -1;
                    } else {
                        i--; //
                    }
                    state++;
                    break;
                case 11:
                case 20:
                case 30:
                case 40:
                case 45:
                    while(raw_data[i] >= '0' && raw_data[i] <= '9') {
                        size *=10;
                        size += raw_data[i] - '0';
                        i++;
                    }
                    state++;
                    if (state == 12) { //return integer;
                        res = size*sign;
                        return {i+1, res};
                    }
                    break;
                case 21: //
                    i++;
                    res = std::string_view(raw_data.substr(i,size));
                    return {i+size+2,res};
                    break;
                case 31: {
                    i++;
                    std::vector<std::pair<nlohmann::json, nlohmann::json>> _array;
                    bool listType = true;
                    for (; size > 0; size--) {
                        auto _key = unserialize(raw_data.substr(i));

                        auto _value = unserialize(raw_data.substr(i + _key.first));
                        if (raw_data[i] == 's') listType = false;
                        _array.push_back({move(_key.second), move(_value.second)});
                        i += _key.first + _value.first;
                    }
                    if (listType) {
                        res = nlohmann::json::array();
                        for (auto &v: _array) {
                            res[v.first.get<int>()] = v.second;
                        }
                    } else {
                        res = nlohmann::json::object();
                        for (auto &v: _array) {
                            std::string k = (v.first.is_string() ?
                                             v.first.get<std::string>() :
                                             std::to_string(v.first.get<int>()));
                            res[k] = v.second;
                        }
                    }
//                    res = _array;
                    return {i + 1, res};
                }
                    break;
                case 41: {
                    i++;
                    res = nlohmann::json::object();
                    res["__type__"] = std::string_view(raw_data.substr(i,size));
                    i+=size+1;
                    state = 45;
                    size=0;
                } break;

                case 46: {
                    i++;
                    for (; size > 0; size--) {
                        auto _key = unserialize(raw_data.substr(i));

                        auto _value = unserialize(raw_data.substr(i + _key.first));
                        // std::cout << _key.second.dump() << std::endl;
                        res[_key.second.get<std::string>()] = move(_value.second);
                        i += _key.first + _value.first;
                    }
                    return {i + 1, res};
                }
                    break;
            }

        }

        return {0,res};
    }

    nlohmann::json unserialize(const std::string& raw_data) {
        return unserialize(std::string_view(raw_data));
    }

    nlohmann::json sessionUnserialize(std::string_view raw_data) {
        nlohmann::json res = nlohmann::json::object();
        std::string key;
        int state = 0;
        int start = 0;

        std::pair<int, nlohmann::json> val;
        for (int i = 0; i < raw_data.size(); i++) {
            switch (state) {
                case 0:
                    start = i;
                    while (i < raw_data.size() && raw_data[i] != '|') i++;
                    key = raw_data.substr(start, i - start);
                    state = 1;
                    break;
                case 1:
                    val = unserialize(raw_data.substr(i));
                    res[key] = val.second;
                    i+=val.first-1;
                    state = 0;
                    break;
            }
        }

        return res;
    }
    nlohmann::json sessionUnserialize(const std::string& raw_data) {
        return sessionUnserialize(std::string_view(raw_data));
    }
}