#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>  //copy_to/from_user()
#include <linux/gpio.h>     //GPIO
#include <linux/err.h>

static int RELAY[] = {-1, 6, 5, 22, 27};

static struct proc_dir_entry *driver_proc_relay_1 = NULL;
static struct proc_dir_entry *driver_proc_relay_2 = NULL;
static struct proc_dir_entry *driver_proc_relay_3 = NULL;
static struct proc_dir_entry *driver_proc_relay_4 = NULL;
static struct proc_dir_entry *driver_proc_all_relay = NULL;

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
 
 
/*************** Driver functions **********************/

static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);

/* RELAY_1*/
static ssize_t etx_read_1(struct file *filp, 
                char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write_1(struct file *filp, 
                const char *buf, size_t len, loff_t * off);
/* RELAY_2*/
static ssize_t etx_read_2(struct file *filp, 
                char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write_2(struct file *filp, 
                const char *buf, size_t len, loff_t * off);
/* RELAY_3*/
static ssize_t etx_read_3(struct file *filp, 
                char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write_3(struct file *filp, 
                const char *buf, size_t len, loff_t * off);
/* RELAY_4*/
static ssize_t etx_read_4(struct file *filp, 
                char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write_4(struct file *filp, 
                const char *buf, size_t len, loff_t * off);
/* RELAY_FULL_CONTROL*/
static ssize_t etx_read_full(struct file *filp, 
                char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write_full(struct file *filp, 
                const char *buf, size_t len, loff_t * off);

/******************************************************/
 
/**************File operation structure****************/

/* RELAY_1 */
static struct proc_ops fops_1 =
{
  .proc_read           = etx_read_1,
  .proc_write          = etx_write_1,
  .proc_open           = etx_open,
  .proc_release        = etx_release,
};

/* RELAY_2 */
static struct proc_ops fops_2 =
{
  .proc_read           = etx_read_2,
  .proc_write          = etx_write_2,
  .proc_open           = etx_open,
  .proc_release        = etx_release,
};

/* RELAY_3 */
static struct proc_ops fops_3 =
{
  .proc_read           = etx_read_3,
  .proc_write          = etx_write_3,
  .proc_open           = etx_open,
  .proc_release        = etx_release,
};

/* RELAY_4 */
static struct proc_ops fops_4 =
{
  .proc_read           = etx_read_4,
  .proc_write          = etx_write_4,
  .proc_open           = etx_open,
  .proc_release        = etx_release,
};

/* FULL_CONTROL */
static struct proc_ops fops_fc =
{
  .proc_read           = etx_read_full,
  .proc_write          = etx_write_full,
  .proc_open           = etx_open,
  .proc_release        = etx_release,
};

/*******************************************************/
/*
** This function will be called when we open the Proc file 
*/ 
static int etx_open(struct inode *inode, struct file *file)
{
  pr_info("Proc file Opened...!!!\n");
  return 0;
}

/*
** This function will be called when we close the Proc file 
*/
static int etx_release(struct inode *inode, struct file *file)
{
  pr_info("Proc file Closed...!!!\n");
  return 0;
}


/**************************************************************/

static ssize_t etx_read_1(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
  uint8_t gpio_state = 0;
  
  gpio_state = gpio_get_value(RELAY[1]);
  
  len = 1;
  if( copy_to_user(buf, &gpio_state, len) > 0) {
    pr_err("ERROR: Not all the Relay1's bytes have been copied to user\n");
  }
  
  pr_info("Read function : RELAY_1 = %d \n", gpio_state);
  
  return 0;
}
 
static ssize_t etx_write_1(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
  uint8_t rec_buf[10] = {0};
  
  if( copy_from_user( rec_buf, buf, len ) > 0) {
    pr_err("ERROR: Not all the Relay1's bytes have been copied from user\n");
  }
  
  pr_info("Write Function : RELAY_1 Set = %c\n", rec_buf[0]);
  
  if (rec_buf[0]=='1' || strncmp(rec_buf,"on",2)==0) {
    gpio_set_value(RELAY[1], 1);
    pr_info("Relay 1: ON");
  } 
  else if (rec_buf[0]=='0' || strncmp(rec_buf,"off",3)==0) {
    gpio_set_value(RELAY[1], 0);
    pr_info("Relay 1: OFF");
  } 
  else {
    pr_err("Unknown command : Please provide one of the following valid values: 1 | 0 | on | off.\n");
  }
  
  return len;
}

/**************************************************************/
 
static ssize_t etx_read_2(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
  uint8_t gpio_state = 0;
  
  gpio_state = gpio_get_value(RELAY[2]);
  
  len = 1;
  if( copy_to_user(buf, &gpio_state, len) > 0) {
    pr_err("ERROR: Not all the Relay2's bytes have been copied to user\n");
  }
  
  pr_info("Read function : RELAY_2 = %d \n", gpio_state);
  
  return 0;
}

static ssize_t etx_write_2(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
  uint8_t rec_buf[10] = {0};
  
  if( copy_from_user( rec_buf, buf, len ) > 0) {
    pr_err("ERROR: Not all the bytes have been copied from user\n");
  }
  
  pr_info("Write Function : RELAY_2 Set = %c\n", rec_buf[0]);
  
  if (rec_buf[0]=='1' || strncmp(rec_buf,"on",2)==0) {
    gpio_set_value(RELAY[2], 1);
    pr_info("Relay 2: ON");
  } 
  else if (rec_buf[0]=='0' || strncmp(rec_buf,"off",3)==0) {
    gpio_set_value(RELAY[2], 0);
    pr_info("Relay 2: OFF");
  } 
  else {
    pr_err("Unknown command : Please provide one of the following valid values: 1 | 0 | on | off. \n");
  }
  
  return len;
}

/**************************************************************/

static ssize_t etx_read_3(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
  uint8_t gpio_state = 0;
  
  gpio_state = gpio_get_value(RELAY[3]);
  
  len = 1;
  if( copy_to_user(buf, &gpio_state, len) > 0) {
    pr_err("ERROR: Not all the Relay3's bytes have been copied to user\n");
  }
  
  pr_info("Read function : RELAY_3 = %d \n", gpio_state);
  
  return 0;
}

static ssize_t etx_write_3(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
  uint8_t rec_buf[10] = {0};
  
  if( copy_from_user( rec_buf, buf, len ) > 0) {
    pr_err("ERROR: Not all the Relay3's bytes have been copied from user\n");
  }
  
  if (rec_buf[0]=='1' || strncmp(rec_buf,"on",2)==0) {
    gpio_set_value(RELAY[3], 1);
    pr_info("Relay 3: ON");
  } 
  else if (rec_buf[0]=='0' || strncmp(rec_buf,"off",3)==0) {
    gpio_set_value(RELAY[3], 0);
    pr_info("Relay 3: OFF");
  } 
  else {
    pr_err("Unknown command : Please provide one of the following valid values: 1 | 0 | on | off.\n");
  }
  
  return len;
}

/**************************************************************/
 
static ssize_t etx_read_4(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
  uint8_t gpio_state = 0;
  
  gpio_state = gpio_get_value(RELAY[4]);
  
  len = 1;
  if( copy_to_user(buf, &gpio_state, len) > 0) {
    pr_err("ERROR: Not all the Relay4's bytes have been copied to user\n");
  }
  
  pr_info("Read function : RELAY_4 = %d \n", gpio_state);
  
  return 0;
}

static ssize_t etx_write_4(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
  uint8_t rec_buf[10] = {0};
  
  if( copy_from_user( rec_buf, buf, len ) > 0) {
    pr_err("ERROR: Not all the bytes have been copied from user\n");
  }
  
  if (rec_buf[0]=='1' || strncmp(rec_buf,"on",2)==0) {
    gpio_set_value(RELAY[4], 1);
    pr_info("Relay 4: ON");
  } 
  else if (rec_buf[0]=='0' || strncmp(rec_buf,"off",2)==0) {
    gpio_set_value(RELAY[4], 0);
    pr_info("Relay 4: OFF");
  } 
  else {
    pr_err("Unknown command : Please provide one of the following valid values: 1 | 0 | on | off.\n");
  }
  
  return len;
}

/**************************************************************/
 
static ssize_t etx_read_full(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
  uint8_t gpio_state[4] = {0};
  
  gpio_state[0] = gpio_get_value(RELAY[1]);
  gpio_state[1] = gpio_get_value(RELAY[2]);
  gpio_state[2] = gpio_get_value(RELAY[3]);
  gpio_state[3] = gpio_get_value(RELAY[4]);

  len = 4;
  if( copy_to_user(buf, &gpio_state, len) > 0) {
    pr_err("ERROR: Not all the Relay's bytes have been copied to user\n");
  }
  
  pr_info("Read function : RELAY = %s \n", gpio_state);
  
  return 0;
}

static ssize_t etx_write_full(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
  uint8_t rec_buf[10] = {0};
  int i;
  if( copy_from_user( rec_buf, buf, len ) > 0) {
    pr_err("ERROR: Not all the bytes have been copied from user\n");
  }
  else{
    pr_info("Copy done: %s", rec_buf);
  }
  if(strncmp(rec_buf,"on",2)==0){
    for( i = 0; i < 4; i++){
	    gpio_set_value(RELAY[i+1], 1);
      pr_info("Turn on all relay");
    }
  }
  else if (strncmp(rec_buf,"off",3)==0){
    for( i = 0; i < 4; i++){
	gpio_set_value(RELAY[i+1], 0);
	pr_info("Turn off all relay");
    }
  }
  else{
    for(i = 0; i < 4; i++){
      if (rec_buf[i]=='1') {
        gpio_set_value(RELAY[i+1], 1);
        pr_info("Relay %d: ON",i+1);
      } 
      else if (rec_buf[i]=='0') {
        gpio_set_value(RELAY[i+1], 0);
        pr_info("Relay %d: OFF",i+1);
      } 
      else {
        pr_err("Error format: %c.Please provide the correct 4 bits in order from relay 1 to relay 4(Example: 0000).\n", rec_buf[i]);
      }
    }
  }
  return len;
}

/***************************************************************/

/*
** Module Init function
*/ 
static int __init etx_driver_init(void)
{
  //Create proc file
  driver_proc_relay_1 = proc_create("relay_1",0666,NULL, &fops_1);
  if (driver_proc_relay_1 == NULL){
    pr_err("Cannot create the Proc file");
    goto r_proc_1;
  }
  else{
    pr_info("Success to create proc file: /proc/relay_1");
  }

  driver_proc_relay_2 = proc_create("relay_2",0666,NULL, &fops_2);
  if (driver_proc_relay_2 == NULL){
    pr_err("Cannot create the Proc file");
    goto r_proc_2;
  }
  else{
    pr_info("Success to create proc file: /proc/relay_2");
  }

  driver_proc_relay_3 = proc_create("relay_3",0666,NULL, &fops_3);
  if (driver_proc_relay_3 == NULL){
    pr_err("Cannot create the Proc file");
    goto r_proc_3;
  }
  else{
    pr_info("Success to create proc file: /proc/relay_3");
  }

  driver_proc_relay_4 = proc_create("relay_4",0666,NULL, &fops_4);
  if (driver_proc_relay_4 == NULL){
    pr_err("Cannot create the Proc file");
    goto r_proc_4;
  }
  else{
    pr_info("Success to create proc file: /proc/relay_4");
  }

  driver_proc_all_relay = proc_create("relay_all",0666,NULL, &fops_fc);
  if (driver_proc_all_relay==NULL){
    pr_err("Cannot create the Proc file");
    goto r_proc_all;
  }
  else{
    pr_info("Success to create proc file: /proc/relay_all");
  }

  //Checking the GPIO is valid or not
  if(gpio_is_valid(RELAY[1]) == false){
    pr_err("GPIO %d is not valid\n", RELAY[1]);
    goto r_proc_1;
  }

  if(gpio_is_valid(RELAY[2]) == false){
    pr_err("GPIO %d is not valid\n", RELAY[2]);
    goto r_proc_2;
  }

  if(gpio_is_valid(RELAY[3]) == false){
    pr_err("GPIO %d is not valid\n", RELAY[3]);
    goto r_proc_3;
  }

  if(gpio_is_valid(RELAY[4]) == false){
    pr_err("GPIO %d is not valid\n", RELAY[4]);
    goto r_proc_4;
  }
  
  //Requesting the GPIO
  if(gpio_request(RELAY[1],"RELAY_1") < 0){
    pr_err("ERROR: GPIO %d request\n", RELAY[1]);
    goto r_gpio_1;
  }
  
  if(gpio_request(RELAY[2],"RELAY_2") < 0){
    pr_err("ERROR: GPIO %d request\n", RELAY[2]);
    goto r_gpio_2;
  }

  if(gpio_request(RELAY[3],"RELAY_3") < 0){
    pr_err("ERROR: GPIO %d request\n", RELAY[3]);
    goto r_gpio_3;
  }

  if(gpio_request(RELAY[4],"RELAY_4") < 0){
    pr_err("ERROR: GPIO %d request\n", RELAY[4]);
    goto r_gpio_4;
  }

  //configure the GPIO as output
  gpio_direction_output(RELAY[1], 0);
  gpio_direction_output(RELAY[2], 0);
  gpio_direction_output(RELAY[3], 0);
  gpio_direction_output(RELAY[4], 0);
  
  //Enable debug
  gpio_export(RELAY[1], false);
  gpio_export(RELAY[2], false);
  gpio_export(RELAY[3], false);
  gpio_export(RELAY[4], false);
  
  pr_info(" Driver Setup...Done!!!\n");
  return 0;

r_gpio_4:
  gpio_free(RELAY[4]);
r_gpio_3:
  gpio_free(RELAY[3]);
r_gpio_2:
  gpio_free(RELAY[2]);
r_gpio_1:
  gpio_free(RELAY[1]);

r_proc_all:
  proc_remove(driver_proc_all_relay);
r_proc_4:
  proc_remove(driver_proc_relay_4);
r_proc_3:
  proc_remove(driver_proc_relay_3);
r_proc_2:
  proc_remove(driver_proc_relay_2);
r_proc_1:
  proc_remove(driver_proc_relay_1);

  return -1;
}

/*
** Module exit function
*/ 
static void __exit etx_driver_exit(void)
{
  gpio_unexport(RELAY[1]);
  gpio_unexport(RELAY[2]);
  gpio_unexport(RELAY[3]);
  gpio_unexport(RELAY[4]);

  gpio_free(RELAY[1]);
  gpio_free(RELAY[2]);
  gpio_free(RELAY[3]);
  gpio_free(RELAY[4]);
  
  proc_remove(driver_proc_relay_1);
  proc_remove(driver_proc_relay_2);
  proc_remove(driver_proc_relay_3);
  proc_remove(driver_proc_relay_4);
  proc_remove(driver_proc_all_relay);

  pr_info("Driver Remove...Done!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thang_20139015");
MODULE_DESCRIPTION("Relay driver");
MODULE_VERSION("1.32");
