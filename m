       �  �         �A  tests ����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �  �       ��  main_test.sh ���������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �         �A  Study ����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �         �A  Docker ���������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �         �A  TensorflowGPU ��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �        ��  Dockerfile �����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �         �A  Haskell ��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �  �       ��  baby.hs ��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �  �      ��  pascal.hs ������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �  �       ��  qux.hs ���������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �  0      ��  karpekar.hs ����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �         �A  PostgreSQL �����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �  :	      ��  Dockerfile �����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �         �A  Study ����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������  �         �A  Docker �����������������������������������������������������������������������������������������������������������������������������������������������������                                                                                                   �  �         �A  TensorflowGPU                                                                                                                                                                                                                                                  �  �        ��  Dockerfile                                                                                                                                                                                                                                                     �  �         �A  Haskell                                                                                                                                                                                                                                                        �  �  �       ��  baby.hs                                                                                                                                                                                                                                                        �  �  �      ��  pascal.hs                                                                                                                                                                                                                                                      �  �  �       ��  qux.hs                                                                                                                                                                                                                                                         �  �  0      ��  karpekar.hs                                                                                                                                                                                                                                                    �  �         �A  PostgreSQL                                                                                                                                                                                                                                                     �  �  :	      ��  Dockerfile                                                                                                                                                                                                                                                     �  �  �      ��  tree.c                                                                                                                                                                                                                                                         �                             ^                            ^^^              ^	       ^
       ^       ^^              ^^                            ^^^              ^       ^       ^       ^^              ^^^^       ^^^#!/usr/bin/bash


../archiver c archiv Study
mv Study Study.old
../archiver x archiv
diff -r Study Study.old
rm -rf Study
rm -f archiv
mv Study.old Study
FROM tensorflow/tensorflow:latest-gpu-py3-jupyter
# this dockerfile is used for tensorflow-gpu containers
MAINTAINER S. Guliaev <sguliaev@moroshka.cloud>
EXPOSE 8888
RUN useradd -m tensorflow
USER tensorflow
ENTRYPOINT ["jupyter-notebook"]
CMD ["--ip=0.0.0.0"]
doubleUs x y = doubleMe x + doubleMe y
doubleMe x = x + x

doubleSmallNumber x = if x > 100
                      then x
                      else doubleMe x
factorial :: Integer -> Integer
factorial 0 = 1
factorial n = product[1..n]

arrange :: Integer -> Integer -> Integer
arrange k n = product[(k+1)..n]

binominal :: Integer -> Integer -> Integer
binominal k n = if k > (n-k)
                then arrange k n `div` factorial (n-k)
                else arrange (n-k) n `div` factorial k

pascal :: Integer -> [Integer]
pascal k = [binominal m (k-1) | m <- [0..(k-1)]]
import Data,List (nub)

quzNum :: [Char] -> Integer
quzNum q
       | unique == 1 = length q
       | otherwise = 0
       where unique = length . nub . q
import Data.List (sort)

karpekar :: Int -> Int
karpekar a
         | a == 6174 = 0
         | otherwise = 1 + karpekar (acsending - descending)
         where
           arr = sort $ (show a)
           acsending = read arr :: Int
	   reversed = reverse arr
           descending = read reversed :: Int
FROM ubuntu

RUN yes | apt update
RUN yes | apt upgrade
RUN yes | apt install gnupg2

# Add the PostgreSQL PGP key to verify their Debian packages.
# It should be the same key as https://www.postgresql.org/media/keys/ACCC4CF8.asc
RUN apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys B97B0AFCAA1A47F044F244A07FCC7D46ACCC4CF8

# Add PostgreSQL's repository. It contains the most recent stable release
#     of PostgreSQL, ``9.3``.
RUN echo "deb http://apt.postgresql.org/pub/repos/apt/ precise-pgdg main" > /etc/apt/sources.list.d/pgdg.list

# Install ``python-software-properties``, ``software-properties-common`` and PostgreSQL 9.3
#  There are some warnings (in red) that show up during the build. You can hide
#  them by prefixing each apt-get statement with DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y python-software-properties software-properties-common postgresql-9.3 postgresql-client-9.3 postgresql-contrib-9.3

# Note: The official Debian and Ubuntu images automatically ``apt-get clean``
# after each ``apt-get``

# Run the rest of the commands as the ``postgres`` user created by the ``postgres-9.3`` package when it was ``apt-get installed``
USER postgres

# Create a PostgreSQL role named ``docker`` with ``docker`` as the password and
# then create a database `docker` owned by the ``docker`` role.
# Note: here we use ``&&\`` to run commands one after the other - the ``\``
#       allows the RUN command to span multiple lines.
RUN    /etc/init.d/postgresql start &&\
    psql --command "CREATE USER docker WITH SUPERUSER PASSWORD 'docker';" &&\
    createdb -O docker docker

# Adjust PostgreSQL configuration so that remote connections to the
# database are possible.
RUN echo "host all  all    0.0.0.0/0  md5" >> /etc/postgresql/9.3/main/pg_hba.conf

# And add ``listen_addresses`` to ``/etc/postgresql/9.3/main/postgresql.conf``
RUN echo "listen_addresses='*'" >> /etc/postgresql/9.3/main/postgresql.conf

# Expose the PostgreSQL port
EXPOSE 5432

# Add VOLUMEs to allow backup of config, logs and databases
VOLUME  ["/etc/postgresql", "/var/log/postgresql", "/var/lib/postgresql"]

# Set the default command to run when starting the container
CMD ["/usr/lib/postgresql/9.3/bin/postgres", "-D", "/var/lib/postgresql/9.3/main", "-c", "config_file=/etc/postgresql/9.3/main/postgresql.conf"]

FROM tensorflow/tensorflow:latest-gpu-py3-jupyter
# this dockerfile is used for tensorflow-gpu containers
MAINTAINER S. Guliaev <sguliaev@moroshka.cloud>
EXPOSE 8888
RUN useradd -m tensorflow
USER tensorflow
ENTRYPOINT ["jupyter-notebook"]
CMD ["--ip=0.0.0.0"]
doubleUs x y = doubleMe x + doubleMe y
doubleMe x = x + x

doubleSmallNumber x = if x > 100
                      then x
                      else doubleMe x
factorial :: Integer -> Integer
factorial 0 = 1
factorial n = product[1..n]

arrange :: Integer -> Integer -> Integer
arrange k n = product[(k+1)..n]

binominal :: Integer -> Integer -> Integer
binominal k n = if k > (n-k)
                then arrange k n `div` factorial (n-k)
                else arrange (n-k) n `div` factorial k

pascal :: Integer -> [Integer]
pascal k = [binominal m (k-1) | m <- [0..(k-1)]]
import Data,List (nub)

quzNum :: [Char] -> Integer
quzNum q
       | unique == 1 = length q
       | otherwise = 0
       where unique = length . nub . q
import Data.List (sort)

karpekar :: Int -> Int
karpekar a
         | a == 6174 = 0
         | otherwise = 1 + karpekar (acsending - descending)
         where
           arr = sort $ (show a)
           acsending = read arr :: Int
	   reversed = reverse arr
           descending = read reversed :: Int
FROM ubuntu

RUN yes | apt update
RUN yes | apt upgrade
RUN yes | apt install gnupg2

# Add the PostgreSQL PGP key to verify their Debian packages.
# It should be the same key as https://www.postgresql.org/media/keys/ACCC4CF8.asc
RUN apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys B97B0AFCAA1A47F044F244A07FCC7D46ACCC4CF8

# Add PostgreSQL's repository. It contains the most recent stable release
#     of PostgreSQL, ``9.3``.
RUN echo "deb http://apt.postgresql.org/pub/repos/apt/ precise-pgdg main" > /etc/apt/sources.list.d/pgdg.list

# Install ``python-software-properties``, ``software-properties-common`` and PostgreSQL 9.3
#  There are some warnings (in red) that show up during the build. You can hide
#  them by prefixing each apt-get statement with DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y python-software-properties software-properties-common postgresql-9.3 postgresql-client-9.3 postgresql-contrib-9.3

# Note: The official Debian and Ubuntu images automatically ``apt-get clean``
# after each ``apt-get``

# Run the rest of the commands as the ``postgres`` user created by the ``postgres-9.3`` package when it was ``apt-get installed``
USER postgres

# Create a PostgreSQL role named ``docker`` with ``docker`` as the password and
# then create a database `docker` owned by the ``docker`` role.
# Note: here we use ``&&\`` to run commands one after the other - the ``\``
#       allows the RUN command to span multiple lines.
RUN    /etc/init.d/postgresql start &&\
    psql --command "CREATE USER docker WITH SUPERUSER PASSWORD 'docker';" &&\
    createdb -O docker docker

# Adjust PostgreSQL configuration so that remote connections to the
# database are possible.
RUN echo "host all  all    0.0.0.0/0  md5" >> /etc/postgresql/9.3/main/pg_hba.conf

# And add ``listen_addresses`` to ``/etc/postgresql/9.3/main/postgresql.conf``
RUN echo "listen_addresses='*'" >> /etc/postgresql/9.3/main/postgresql.conf

# Expose the PostgreSQL port
EXPOSE 5432

# Add VOLUMEs to allow backup of config, logs and databases
VOLUME  ["/etc/postgresql", "/var/log/postgresql", "/var/lib/postgresql"]

# Set the default command to run when starting the container
CMD ["/usr/lib/postgresql/9.3/bin/postgres", "-D", "/var/lib/postgresql/9.3/main", "-c", "config_file=/etc/postgresql/9.3/main/postgresql.conf"]

// SPDX-License-Identifier: GPL-2.0

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smartstr.h"
#include "tree.h"

int	main(void)
{
	t_tree	*tree;
	t_tree	*tree_2;
	t_tree	*tmp;
	t_smartstr *sstr;

	tree = init_tree(0);
	insert_child(tree, 1);
	insert_child(tree, 2);
	tmp = tree->children[0];
	insert_child(tmp, 3);
	sstr = serialize_tree(tree);
	tree_2 = deserialize_tree(sstr);
	if (tree_2->value != 0 || tree_2->children_count != 2 || 
	    tree_2->children[0]->value != 1 || tree_2->children[0]->children_count != 1 ||
	    tree_2->children[1]->value != 2 || tree_2->children[1]->children_count != 0 ||
	    tree_2->children[0]->children[0]->value !=3 ||
	    tree_2->children[0]->children[0]->children_count != 0) {
		printf("%s\n", "Test failed");
		printf("Tree root value; %lu\n", tree_2->value);
		printf("Children count root value; %lu\n", tree_2->children_count);
		printf("Tree children 0 value; %lu\n", tree_2->children[0]->value);
		printf("Children count children 0 value; %lu\n",
				tree_2->children[0]->children_count);
		printf("Tree children 1 value; %lu\n", tree_2->children[1]->value);
		printf("Children count children 1 value; %lu\n",
				tree_2->children[1]->children_count);
		printf("Tree children 0 children 0 value; %lu\n",
				tree_2->children[0]->children[0]->value);
		printf("Children count children 0 children 0 value; %lu\n",
				tree_2->children[0]->children[0]->children_count);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
