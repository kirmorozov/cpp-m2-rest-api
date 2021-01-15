Baseline Magento API using C++
==========
Code is proof of concept of Magento Rest API implementation using [Pistache C++ Framework](https://github.com/pistacheio/pistache)
Communication with MySql is done though [XDevAPI](https://dev.mysql.com/doc/dev/connector-cpp/8.0/devapi_ref.html)

Admin login (`index.php/rest/V1/integration/admin/token`) works using libsodium, with same encryption as within Magento Core. 
Primitive ACL validation added for some basic GET methods implemented.

Performance 
===========
Other metrics were not checked, better to check with Magento original Rest Test suite.
For example authorization as it was implemented takes <100ms in comparison to original 200ms+.
Simple methods like `store/storeViews` do not have too much overhead, and perform within 20ms.

```shell
$ time curl -X POST "http://127.0.0.1:8080/index.php/rest/V1/integration/admin/token" \
    -H "Content-Type:application/json"      -d '{"username":"admin", "password":"123123qa"}'
"a1ie9y2g8dcuuqoxe5oqm8b3dxmf8yz6"
real	0m0.097s
user	0m0.005s
sys	0m0.005s
$ time curl -X POST "http://127.0.0.1:8080/index.php/rest/V1/integration/admin/token" \
    -H "Content-Type:application/json"      -d '{"username":"admin", "password":"123123qa"}'
"4bncvfc03cntrqyv1jte8evcug2ogf7q"
real	0m0.098s
user	0m0.005s
sys	0m0.005s
$ time curl -X GET "http://127.0.0.1:8080/index.php/rest/V1/store/storeViews"      \
    -H "Authorization: Bearer 4bncvfc03cntrqyv1jte8evcug2ogf7q"
[{"code":"admin","id":0,"is_active":1,"name":"Admin","store_group_id":0,"website_id":0},{"code":"default","id":1,"is_active":1,"name":"Default Store View","store_group_id":1,"website_id":1}]
real	0m0.047s
user	0m0.004s
sys	0m0.007s
$ time curl -X GET "http://127.0.0.1:8080/index.php/rest/V1/store/storeViews"      
    -H "Authorization: Bearer 4bncvfc03cntrqyv1jte8evcug2ogf7q"
[{"code":"admin","id":0,"is_active":1,"name":"Admin","store_group_id":0,"website_id":0},{"code":"default","id":1,"is_active":1,"name":"Default Store View","store_group_id":1,"website_id":1}]
real	0m0.025s
user	0m0.005s
sys	0m0.007s
$ time curl -X GET "http://127.0.0.1:8080/index.php/rest/V1/store/storeViews"      \
    -H "Authorization: Bearer 4bncvfc03cntrqyv1jte8evcug2ogf7q"
[{"code":"admin","id":0,"is_active":1,"name":"Admin","store_group_id":0,"website_id":0},{"code":"default","id":1,"is_active":1,"name":"Default Store View","store_group_id":1,"website_id":1}]
real	0m0.019s
user	0m0.005s
sys	0m0.005s
$ time curl -X GET "http://127.0.0.1:8080/index.php/rest/V1/store/storeViews"      
    -H "Authorization: Bearer 4bncvfc03cntrqyv1jte8evcug2ogf7q"
[{"code":"admin","id":0,"is_active":1,"name":"Admin","store_group_id":0,"website_id":0},{"code":"default","id":1,"is_active":1,"name":"Default Store View","store_group_id":1,"website_id":1}]
real	0m0.027s
user	0m0.005s
sys	0m0.006s
```

Future
======
There may be some future with the code, it can be taken as baseline to implement high-performing apis.
GraphQL can be added later, as cpp graphql library is available.

License
=======
    Copyright 2021 Kirill Morozov <kir.morozov@gmail.com>

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
