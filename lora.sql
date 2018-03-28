DROP DATABASE IF EXISTS lora;
CREATE DATABASE lora DEFAULT CHARACTER SET utf8;
USE lora;

CREATE TABLE user(
    id int(11) NOT NULL AUTO_INCREMENT,
    name varchar(255) DEFAULT NULL,
    password varchar(255) DEFAULT NULL,
    PRIMARY KEY (id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE address(
    id int(11) NOT NULL AUTO_INCREMENT,
    address_name varchar(255) DEFAULT NULL,
    PRIMARY KEY(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE room(
    id int(11) NOT NULL AUTO_INCREMENT,
    cid int(11) DEFAULT NULL,
    room_name varchar(255) DEFAULT NULL,
    PRIMARY KEY (id),
    CONSTRAINT room_address FOREIGN KEY (cid) REFERENCES address (id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE appliance(
    id int(11) NOT NULL AUTO_INCREMENT,
    appliance_id varchar(255) DEFAULT NULL,
    remark varchar(255) DEFAULT NULL,
    did int(11) DEFAULT NULL,
    eid int(11) DEFAULT NULL,
    Dates datetime DEFAULT NULL,
    PRIMARY KEY (id),
    CONSTRAINT appliance_address FOREIGN KEY(did) REFERENCES address (id),
    CONSTRAINT appliance_room FOREIGN KEY(eid) REFERENCES room (id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE property(
    id int(11) NOT NULL AUTO_INCREMENT,
    useapp int(11) DEFAULT NULL,
    light int(11) DEFAULT NULL,
    fire int(11) DEFAULT NULL,
    pm25 int(11) DEFAULT NULL,
    humidity int(11) DEFAULT NULL,
    temperature int(11) DEFAULT NULL,
    pid int(11) DEFAULT NULL,
    tid int(11) DEFAULT NULL,
    PRIMARY KEY(id),
    CONSTRAINT property_address FOREIGN KEY (pid) REFERENCES address (id),
    CONSTRAINT property_room FOREIGN KEY (tid) REFERENCES room (id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE img(
    id int(11) NOT NULL AUTO_INCREMENT,
    pid int(11) DEFAULT NULL,
    type varchar(255) DEFAULT NULL,
    PRIMARY KEY (id),
    CONSTRAINT img_appliance FOREIGN KEY (pid) REFERENCES appliance (id)
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
