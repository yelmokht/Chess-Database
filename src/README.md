To install the chess extension: 

```bash
make
sudo make install
sudo -u postgres createuser -e your_username
sudo -u postgres psql -c "ALTER USER your_username SUPERUSER;"
createdb chess
psql chess
```
And in psql :

```sql
CREATE EXTENSION chess;
\dx -- To verify the existence of the extension
-- TODO
```
