# Replace 'yourprogram.exe' with the path to your executable file
$executable = ".\03.exe"

# Measure the execution time
$time = Measure-Command {
    & $executable
}

# Output the execution time
Write-Output "Execution Time of $($exectuable): $($time.TotalMilliseconds) milliseconds"
