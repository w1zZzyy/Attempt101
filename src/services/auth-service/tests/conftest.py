import pathlib
import pytest
from testsuite.databases.pgsql import discover

pytest_plugins = ['pytest_userver.plugins.postgresql']

@pytest.fixture(scope='session')
def pgsql_local(pgsql_local_create):
    project_root = pathlib.Path(__file__).resolve().parent.parent.parent
    schemas_path = project_root / 'postgresql' / 'schemas'
    if not schemas_path.exists():
        raise RuntimeError(f"Could not find schemas folder at {schemas_path!r}")
    databases = discover.find_schemas('db', [schemas_path])
    if not databases:
        raise RuntimeError(
            f"No schemas found in {schemas_path!r}; "
            "did you name your file 'db.sql' and put a CREATE SCHEMA here?"
        )
    return pgsql_local_create(list(databases.values()))
