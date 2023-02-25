#!/usr/bin/env python3

import json
import ujson
import simplejson as cjson
import time
import utils
LOOP=10
LOOP2=10
files=['file_size2.json', 'file_perm.json', 'in.json', 'file_size.json']
def main():
    datas = [None] * len(files)
    json_doc = [None] * len(files)
    cjson_doc = [None] * len(files)
    ujson_doc = [None] * len(files)
    utils_doc = [None] * len(files)
    i=0
    for filename in files:
        with open(filename, 'r') as file:
            datas[i] = file.read()
            file.close()
            i += 1

    # JSON
    start_ts = time.time()
    for i in range(LOOP):
        j = 0
        for data in datas:
            json_doc[j] = json.loads(data)
            j += 1
    end_ts = time.time()
    print(f" json.loads exec time: {end_ts-start_ts}")
    start_ts = time.time()
    for i in range(LOOP2):
        for json_d in json_doc:
            json_str = json.dumps(json_d)
    end_ts = time.time()
    print(f" json.dumps exec time: {end_ts-start_ts}\n")

    # CJSON
    start_ts = time.time()
    for i in range(LOOP):
        j = 0
        for data in datas:
            cjson_doc[j] = cjson.loads(data)
            j += 1
    end_ts = time.time()
    print(f"cjson.loads exec time: {end_ts-start_ts}")
    start_ts = time.time()
    for i in range(LOOP2):
        for cjson_d in cjson_doc:
            cjson_str = json.dumps(cjson_d)
    end_ts = time.time()
    print(f"cjson.dumps exec time: {end_ts-start_ts}\n")

    # UJSON
    start_ts = time.time()
    for i in range(LOOP):
        j = 0
        for data in datas:
            ujson_doc[j] = ujson.loads(data)
            j += 1
    end_ts = time.time()
    print(f"ujson.loads exec time: {end_ts-start_ts}")
    start_ts = time.time()
    for i in range(LOOP2):
        for ujson_d in ujson_doc:
            ujson_str = json.dumps(ujson_d)
    end_ts = time.time()
    print(f"ujson.dumps exec time: {end_ts-start_ts}\n")
    #print(ujson_str)

    # UTILS.JSON
    start_ts = time.time()
    for i in range(LOOP):
        j = 0
        for data in datas:
            utils_doc[j] = utils.loads(data)
            j += 1
    end_ts = time.time()
    print(f"utils.loads exec time: {end_ts-start_ts}")
    start_ts = time.time()
    for i in range(LOOP2):
        for utils_d in utils_doc:
            #print(utils_d)
            utils_str = utils.dumps(utils_d)
            #print(utils_str)
    end_ts = time.time()
    print(f"utils.dumps exec time: {end_ts-start_ts}\n")

    for i in range(len(json_doc)):
        assert json_doc[i] == ujson_doc[i] == cjson_doc[i] == utils_doc[i]
    for utils_d in utils_doc:
        utils_str = utils.dumps(utils_d)
        assert utils_str == utils.dumps(utils.loads(utils_str))

if __name__ == "__main__":
    main()
