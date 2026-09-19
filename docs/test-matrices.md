# Test Matrices

## Noise Level calculation [20260919]

### Boundary checks

| **ID** | ☑️ |U/I  | Phase | **Scenario**                        | **Setup**    | **Expected** |
| -----  | -- | --- | ----- | ----------------------------------- | ------------ | -------------|
| CFG-01 | ✅ | U   | defaults   | Defaults create valid config      | MigToolConfig::defaults()                    | preflight, staging→local, dry-run true, writes false |

| ☑️ | **raw dB** | **current** | **expected** | **reason**                            |
| -- | ---------- | ----------- | ------------ | ------------------------------------- |
| ✅ | MIN_DB-10  | 0           | 0            | below MIN_DB clamps to minimum        |
| ✅ | MIN_DB     | 0           | 0            | exactly minimum                       |
| ✅ | MIN_DB+1   | 0           | 0            | remains level 0                       |
| ✅ | MAX+DB     | 0           | 8            | maximum                               |
| ✅ | MAX_DB+10  | 0           | 8            | above maximum clamps to maximum       |

### Hysteresis 
| ☑️ | **raw dB** | **current** | **expected** | **reason**                            |
| -- | ---------- | ----------- | ------------ | ------------------------------------- |
| ✅ | 57         | 2           | 0            |                                     |
| ✅ | 59.4       | 2           | 1            | just below lower threshold          |
| ✅ | 59.5       | 2           | 2            | exactly threshold: hold             |
| ✅ | 59.6       | 2           | 2            | just inside hysteresis              |
| ✅ | 59         | 0           | 0            |                                     |
| ✅ | 64         | 2           | 2            |                                     |
| ✅ | 67         | 2           | 2            |                                     |
| ✅ | 69.2       | 2           | 2            | below threshold: hold               |
| ✅ | 69.25      | 2           | 2            | exactly threshold: hold             |
| ✅ | 69.3       | 2           | 3            | above threshold: rise               |

### Rise Limit

| ☑️ | **current** | **target** | **expected** | **behaviour**        |
| -- | ----------- | ---------- | ------------ | -------------------- |
| ✅ | 0           | 0          | 0            | unchanged            |
| ✅ | 2           | 2          | 2            | unchanged            |
| ✅ | 2           | 3          | 3            | rise one             |
| ✅ | 2           | 4          | 3            | rise limited         |
| ✅ | 2           | 8          | 3            | large spike limited  |
| ✅ | 7           | 8          | 8            | rise to maximum      |
| ✅ | 5           | 4          | 4            | fall one             |
| ✅ | 5           | 2          | 2            | large fall immediate |
| ✅ | 8           | 0          | 0            | fall to minimum      |

### Fall Limit

| ☑️ | **current** | **target** | **expected** | **behaviour**        |
| -- | ----------- | ---------- | ------------ | -------------------- |
| ✅ | 0           | 0          | 0            | unchanged            |
| ✅ | 3           | 3          | 3            | unchanged            |
| ✅ | 3           | 2          | 2            | fall one             |
| ✅ | 3           | 1          | 2            | fall limited         |
| ✅ | 8           | 1          | 7            | large spike limited  |
| ✅ | 1           | 0          | 0            | fall to minimum      |
| ✅ | 5           | 6          | 6            | rise one             |
| ✅ | 5           | 2          | 4            | large fall immediate |
| ✅ | 8           | 0          | 7            | fall to minimum      |


### Sequence 
| ☑️ | **current**  | **sequence** | **expected** | **behaviour**        |
| -- | -----------  | ------------ | -------------------- |
|    | 1            | quiet, quiet, bang! quiet | 2; 1    | single loud noise does not max out |
