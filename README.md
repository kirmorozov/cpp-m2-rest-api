Baseline Magento API using C++
==========
Code is proof of concept of Magento Rest API implementation using [Pistache C++ Framework](https://github.com/pistacheio/pistache)
Communication with MySql is done though [XDevAPI](https://dev.mysql.com/doc/dev/connector-cpp/8.0/devapi_ref.html)

Admin login (`index.php/rest/V1/integration/admin/token`) works using libsodium, with same encryption as within Magento Core. 
Primitive ACL validation added for some basic GET methods implemented.

Performance 
===========
Other metrics were not checked, better to check with Magento original Rest Test suite.
For example authorization as it was implemented takes ~70ms in comparison to original 200ms+.

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
