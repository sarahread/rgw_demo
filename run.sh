#!/bin/bash
make clean
make
rm sample.output

exec > >(tee -i sample.output)
exec 2>&1 

echo "# user create"
./radosgw-admin user create --uid=42 --display-name=foo --email=foo@gmail.com
echo

echo "# user delete"
./radosgw-admin user delete --uid=42
echo

echo "# user info"
./radosgw-admin user info --uid=42
echo

echo "# user (no subcommand)"
./radosgw-admin user
echo

echo "# invalid command" 
./radosgw-admin invalidcommand
echo

echo "# invalid option"
./radosgw-admin --invalidoption
echo

echo "# help"
./radosgw-admin --help
echo &&

echo "run finished."