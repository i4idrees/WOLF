FROM gcc:latest

# Install required packages
RUN apt-get update && \
    apt-get install -y \
        cmake g++ gdb make \
        libmariadb-dev libmariadb-dev-compat \
        libtinyxml2-dev \
        wget curl default-jdk valgrind \
        mariadb-client \
        ca-certificates \
        gnupg \
        lsb-release \
    && rm -rf /var/lib/apt/lists/*

# Install Docker CLI
RUN mkdir -p /etc/apt/keyrings && \
    curl -fsSL https://download.docker.com/linux/debian/gpg | gpg --dearmor -o /etc/apt/keyrings/docker.gpg && \
    echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian $(lsb_release -cs) stable" | tee /etc/apt/sources.list.d/docker.list > /dev/null && \
    apt-get update && \
    apt-get install -y docker-ce-cli

# Install Docker Compose
RUN curl -L "https://github.com/docker/compose/releases/download/v2.24.0/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose && \
    chmod +x /usr/local/bin/docker-compose

# Install JMeter
RUN wget -q https://archive.apache.org/dist/jmeter/binaries/apache-jmeter-5.1.1.tgz && \
    tar -xzf apache-jmeter-5.1.1.tgz -C /opt && \
    rm apache-jmeter-5.1.1.tgz

RUN curl -sL https://repo1.maven.org/maven2/mysql/mysql-connector-java/8.0.33/mysql-connector-java-8.0.33.jar \
    -o /opt/apache-jmeter-5.1.1/lib/mysql-connector-java-8.0.33.jar

ENV JMETER_HOME /opt/apache-jmeter-5.1.1
ENV PATH $JMETER_HOME/bin:$PATH

# Work directory
WORKDIR /app/src