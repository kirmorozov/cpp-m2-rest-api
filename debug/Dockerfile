FROM centos/s2i-base-centos8:latest

RUN dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm && \
    dnf install -y https://dev.mysql.com/get/mysql80-community-release-el8-1.noarch.rpm && \
    dnf config-manager --set-enabled PowerTools && \
    yum -y update && \
    yum install -y \
    gcc gcc-c++ openssh-server cmake gdb gdb-gdbserver rsync vim \
    libsodium-devel \
    json-devel \
    rapidjson-devel \
    mysql-connector-c++-devel


RUN mkdir /var/run/sshd && \
    echo 'root:root' | chpasswd && \
    sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config && \
    sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
    # SSH login fix. Otherwise user is kicked off after login

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

RUN cd $APP_ROOT && \
    git clone https://github.com/pistacheio/pistache.git && \
    cd pistache && \
    git submodule update --init && \
    mkdir -p {build,prefix} && \
    cd build && \
    cmake -G "Unix Makefiles" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DPISTACHE_BUILD_EXAMPLES=true \
        -DPISTACHE_BUILD_TESTS=true \
        -DPISTACHE_BUILD_DOCS=false \
        -DPISTACHE_USE_SSL=true \
        ../ && \
    make -j && \
    make install

# 22 for ssh server. 7777 for gdb server.
EXPOSE 22 7777 8080

RUN useradd -ms /bin/bash debugger && \
    echo 'debugger:pwd' | chpasswd

RUN ssh-keygen -b 1024 -t rsa -f /etc/ssh/ssh_host_key && \
    ssh-keygen -b 1024 -t rsa -f /etc/ssh/ssh_host_rsa_key && \
    ssh-keygen -b 1024 -t dsa -f /etc/ssh/ssh_host_dsa_key && \
    rm /run/nologin

CMD ["/usr/sbin/sshd", "-D"]