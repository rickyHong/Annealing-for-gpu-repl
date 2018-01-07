#ifndef SQAOD_CUDA_COPY_H__
#define SQAOD_CUDA_COPY_H__

#include <cuda_runtime.h>
#include <cuda/DeviceMatrix.h>
#include <cuda/DeviceArray.h>
#include <cuda/DeviceStream.h>

namespace sqaod_cuda {

struct DeviceCopy {

    template<class V>
    using HostMatrixType = sqaod::MatrixType<V>;

    template<class V>
    using HostVectorType = sqaod::VectorType<V>;
    
    template<class V>
    void copy(V *d_buf, const V *v, size_t nElms) const;

    template<class V>
    void broadcastCopy(V *d_buf, V v, size_t nElms) const;

    template<class V>
    void scatterBroadcastCopy(V *d_buf, const V &v, size_t size, int stride, int offset) const;

    /* HostMatrix <-> DeviceMatrix */
    
    template<class V>
    void operator()(DeviceMatrixType<V> *dst, const HostMatrixType<V> &src) const;
    
    template<class V>
    void operator()(HostMatrixType<V> *dst, const DeviceMatrixType<V> &src) const;
    
    template<class V>
    void operator()(DeviceMatrixType<V> *dst, const DeviceMatrixType<V> &src) const;
    
    template<class V>
    void operator()(DeviceMatrixType<V> *dst, const V &src) const;
    
    template<class V>
    void operator()(DeviceMatrixType<V> *dst, const V &src, size_t size,
                    int offset, int stride) const;
    
    /* HostVector <-> DeviceVector */
    
    template<class V>
    void operator()(DeviceVectorType<V> *dst, const HostVectorType<V> &src) const;
    
    template<class V>
    void operator()(HostVectorType<V> *dst, const DeviceVectorType<V> &src) const;
    
    template<class V>
    void operator()(DeviceVectorType<V> *dst, const DeviceVectorType<V> &src) const;
    
    template<class V>
    void operator()(DeviceVectorType<V> *dst, const V &src) const;
    
    /* Host scalar variables <-> DeviceScalar */
    
    template<class V>
    void operator()(DeviceScalarType<V> *dst, const V &src) const;
    
    template<class V>
    void operator()(V *dst, const DeviceScalarType<V> &src) const;
    
    /* Packed bits */
    void operator()(sqaod::PackedBitsArray *dst, const DevicePackedBitsArray &src) const;


    DeviceCopy();
    DeviceCopy(DeviceStream &stream);
    
    void setStream(DeviceStream &stream);
    
private:
    DeviceStream *devStream_;
};

template<class V> inline
void DeviceCopy::operator()(HostMatrixType<V> *dst, const DeviceMatrixType<V> &src) const {
    copy(dst->data, src.d_data, src.rows * src.cols);
    dst->rows = src.rows;
    dst->cols = src.cols;
}

template<class V> inline
void DeviceCopy::operator()(DeviceMatrixType<V> *dst, const DeviceMatrixType<V> &src) const {
    copy(dst->d_data, src.d_data, src.rows * src.cols);
    dst->rows = src.rows;
    dst->cols = src.cols;
}

template<class V> inline
void DeviceCopy::operator()(DeviceMatrixType<V> *dst, const V &src) const {
    broadcastCopy(dst->d_dst, src, dst->rows * dst->cols);
}

template<class V> inline
void DeviceCopy::operator()(DeviceMatrixType<V> *dst, const V &src, size_t size,
                            int stride, int offset) const {
    scatterBroadcastCopy(dst->d_data, src, size, stride, offset);
}
    
template<class V> inline
void DeviceCopy::operator()(DeviceVectorType<V> *dst, const HostVectorType<V> &src) const {
    copy(dst->d_data, src.data, src.size);
    dst->size = src.size;
}
    
template<class V> inline
void DeviceCopy::operator()(HostVectorType<V> *dst, const DeviceVectorType<V> &src) const {
    copy(dst->data, src.d_data, src.size);
    dst->size = src.size;
}

template<class V> inline
void DeviceCopy::operator()(DeviceVectorType<V> *dst, const DeviceVectorType<V> &src) const {
    copy(dst->d_data, src.d_data, src.size);
    dst->size = src.size;
}

template<class V> inline
void DeviceCopy::operator()(DeviceVectorType<V> *dst, const V &src) const {
    copy(dst->d_data, src.data, src.size);
}

/* Host scalar variables <-> DeviceScalar */
    
template<class V> inline
void DeviceCopy::operator()(DeviceScalarType<V> *dst, const V &src) const {
    copy(dst->d_data, &src, 1);
}

template<class V> inline
void DeviceCopy::operator()(V *dst, const DeviceScalarType<V> &src) const {
    copy(dst, &src.d_data, 1);
}

/* Packed bits */ inline
void DeviceCopy::operator()(sqaod::PackedBitsArray *dst, const DevicePackedBitsArray &src) const {
    assert(!"not implemented.");
    // copy(dst->data_, src.d_data, src.size);
    // dst->size_ = src.size;
}

}


#endif
