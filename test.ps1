# Replace 'yourprogram.exe' with the path to your executable file
$executable = ".\testing.exe"

# Measure the execution time
$time = Measure-Command {
    & $executable
}

# Output the execution time
Write-Output "Execution Time: $($time.TotalMilliseconds) milliseconds"
