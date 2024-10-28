# pylint: disable=missing-module-docstring
# pylint: disable=missing-class-docstring
# pylint: disable=missing-function-docstring

import os
import shutil

from tqdm import tqdm
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
matplotlib.use('Agg')


def evaluation(model, data_loader, test_name, epoch, save_img=False):
   
    predictions, labels, img_list = inference_all_data(model, data_loader)
    accuracy = calc_accuracy_class(predictions, labels)

    save_accuracy_log(predictions, labels, model, test_name, epoch)
    confusion_matrix = make_confusion_matrix(predictions, labels, model.class_size)
    
    #draw_confusion_matrix(confusion_matrix, test_name, model.labels, model.log_dir, epoch)
    if save_img:
        save_error_images(predictions, labels, img_list,
                          model.labels, model.images_pkl, test_name, model.log_dir)

    return accuracy


def save_best_log(model, test_name, epoch):
    error_base = os.path.join(model.log_dir, test_name, 'logs')
    accuracy_file_name = os.path.join(error_base, '%s_%04d.txt' % (test_name, epoch))
    fig_file_name = os.path.join(error_base, '%s_%04d.jpg' % (test_name, epoch))

    best_name = 'best_result'
    best_base = os.path.join(model.log_dir, best_name, 'logs')
    if not os.path.exists(best_base):
        os.makedirs(best_base)
    best_accuracy_file_name = os.path.join(best_base, '%s_%04d.txt' % (best_name, epoch))
    best_fig_file_name = os.path.join(best_base, '%s_%04d.jpg' % (best_name, epoch))

    shutil.copyfile(accuracy_file_name, best_accuracy_file_name)
    shutil.copyfile(fig_file_name, best_fig_file_name)


def inference_all_data(model, data_loader):
    full_predictions = np.zeros((0, model.class_size))
    full_labels = np.array([])
    full_img_list = np.array([])
    
    model.change_eval_model()
    for _, (images, labels, img_list) in enumerate(tqdm(data_loader)):
        import torch 

        predictions, _ = model(images, labels, is_train=False)
        full_predictions = np.concatenate((full_predictions, predictions))
        full_labels = np.concatenate((full_labels, labels.to('cpu').numpy()))
        full_img_list = np.concatenate((full_img_list, img_list))

    return full_predictions, full_labels, full_img_list

# inout_version_inference code (24.10.21, joo) 
def inference_per_data(model, data_loader):
    full_predictions = np.zeros((0, model.class_size))
    #full_labels = np.array([])
    #full_img_list = np.array([])
    
    model.change_eval_model()
    for _, (images) in enumerate(tqdm(data_loader)):
        import torch 
        labels = np.array([0])
        predictions, _ = model(images, labels, is_train=False)
        full_predictions = np.concatenate((full_predictions, predictions))
        #full_labels = np.concatenate((full_labels, labels.to('cpu').numpy()))
        #full_img_list = np.concatenate((full_img_list, img_list))

    return full_predictions

def calc_accuracy_class(predictions, labels):
    accuracy = 0.0

    tmp_pred_label = []
    tmp_gt_label = [] 
    
    for i in range(labels.shape[0]):
        predict_label = np.argmax(predictions[i])
        label = int(labels[i])

        tmp_pred_label.append(predict_label)
        tmp_gt_label.append(label)

        if predict_label == label:
            accuracy += 1

    accuracy /= labels.shape[0]
    accuracy *= 100
    return accuracy


def save_accuracy_log(predictions, labels, model, test_name, epoch):
    error_base = os.path.join(model.log_dir, test_name, 'logs')
    if not os.path.exists(error_base):
        os.makedirs(error_base)
    accuracy_file_name = os.path.join(error_base, '%s_%04d.txt' % (test_name, epoch))

    correct_cnt_list = [0] * model.class_size
    total_cnt_list = [0] * model.class_size
    for i in range(labels.shape[0]):
        predict_label = np.argmax(predictions[i])
        label = int(labels[i])

        total_cnt_list[label] += 1
        if predict_label == label:
            correct_cnt_list[label] += 1

    log_file = open(accuracy_file_name, 'wt', encoding='utf-8')
    for i in range(model.class_size):
        class_acc = correct_cnt_list[i] / total_cnt_list[i]
        print ("{:<20s} {:6d} {:6d} {:>10.6f}".format(model.labels[i],
                                                      correct_cnt_list[i],
                                                      total_cnt_list[i],
                                                      class_acc))
        save_string = [model.labels[i], correct_cnt_list[i], total_cnt_list[i], class_acc]
        log_file.write('\t'.join(list(map(str, save_string))) + '\n')

    correct_cnt = np.sum(correct_cnt_list)
    total_cnt = np.sum(total_cnt_list)
    total_acc = correct_cnt / total_cnt
    print ("{:<20s} {:6d} {:6d} {:>10.6f}".format('Total',
                                                  correct_cnt,
                                                  total_cnt,
                                                  total_acc))
    save_string = ['Total', correct_cnt, total_cnt, total_acc]
    log_file.write('\t'.join(list(map(str, save_string))))
    log_file.close()


def make_confusion_matrix(predictions, labels, class_size):
    confusion_matrix = np.zeros([class_size, class_size]).astype(np.int64)

    for i in range(labels.shape[0]):
        predict_label = np.argmax(predictions[i])
        label = int(labels[i])
        confusion_matrix[label, predict_label] += 1
    print(confusion_matrix)
    return confusion_matrix


def draw_confusion_matrix(confusion_matrix, test_name, labels, log_dir, epoch):
    plt.rcParams['figure.figsize'] = (20, 20)
    plt.rcParams['image.interpolation'] = 'bilinear'

    norm_conf = []
    for i in confusion_matrix:
        tmp_arr = []
        sum_cnt = sum(i, 0)
        for j in i:
            if sum_cnt == 0:
                tmp_arr.append(0)
            else:
                tmp_arr.append(float(j) / float(sum_cnt))
        norm_conf.append(tmp_arr)

    fig = plt.figure()
    plt.clf()
    sub_plot = fig.add_subplot(111)
    sub_plot.set_aspect(1)
    width, height = confusion_matrix.shape
    res = sub_plot.imshow(np.array(norm_conf), cmap=plt.cm.jet,
                          interpolation='nearest')
    plt.title(log_dir)
    plt.ylabel('True label')
    plt.xlabel('Predicted label')
    for col in range(width):
        for row in range(height):
            sub_plot.annotate(str(confusion_matrix[col][row]), xy=(col, row),
                              horizontalalignment='center',
                              verticalalignment='center', fontsize=15)
    fig.colorbar(res)

    plt.xticks(range(width), labels, fontsize=12)
    plt.yticks(range(height), labels, fontsize=12)
    axes = plt.gca()
    axes.set_ylim([height - 0.5, -0.5])

    error_base = os.path.join(log_dir, test_name, 'logs')
    if not os.path.exists(error_base):
        os.makedirs(error_base)
    fig_file_name = os.path.join(error_base, '%s_%04d.jpg' % (test_name, epoch))
    plt.savefig(fig_file_name, format='jpg')
    plt.close()


def save_error_images(predictions, labels, img_list, images_pkl, labels_name, test_name, log_dir):
    error_base = os.path.join(log_dir, '%s/error_images' % (test_name))
    if os.path.exists(error_base):
        shutil.rmtree(error_base)

    if not os.path.exists(error_base):
        os.makedirs(error_base)

    for i in range(labels.shape[0]):
        predict_label = np.argmax(predictions[i])
        label = int(labels[i])

        if predict_label != label:
            img_path = img_list[i]
            img_name = img_path.split('/')[-1]
            error_filepath = os.path.join(error_base,
                                          labels_name[label],
                                          labels_name[predict_label])
            error_imgpath = os.path.join(error_filepath, img_name)
            if not os.path.exists(error_filepath):
                os.makedirs(error_filepath)
            error_file = open(error_imgpath, 'wb')
            error_file.write(images_pkl[img_path])
            error_file.close()
