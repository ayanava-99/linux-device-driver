def main():
    d=open('/proc/mydriver')
    kernel_msg=d.readline()
    print(kernel_msg)
    d.close()
    return
main()
