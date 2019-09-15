echo "[+] Wiping timestamps"
sudo find /var/www/ -name "*" -exec touch {} \;

echo "[+] Backing up Webroot"
tar -cf webroot.tar /var/www
bzip2 webroot.tar

echo "[+] Backing up MySQL" 
mysqldump -u root -pfluffybunnies wordpress > mysql_backup.sql
bzip2 mysql_backup.sql

echo "[+] Backing up Logs"
tar -cf logs.tar /var/log/
bzip2 logs.tar

echo "[+] Backup configuration"
tar -cf etc_directory.tar /etc
bzip2 etc_directory.tar

echo "[+] Cleaing up temp files"
rm -rf webroot.tar mysql_backup.sql logs.tar
 
